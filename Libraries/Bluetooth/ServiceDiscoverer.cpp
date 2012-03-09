/*
 * Copyright © 2009 Nokia Corporation.
 */

// INCLUDE FILES
#include <btsdp.h>
#include "ServiceDiscoverer.h"

CServiceDiscoverer* CServiceDiscoverer::NewL(MServiceDiscoObserver& aObserver)
    {
    CServiceDiscoverer* self = CServiceDiscoverer::NewLC(aObserver);
    CleanupStack::Pop(self);
    return self;
    }


CServiceDiscoverer* CServiceDiscoverer::NewLC(MServiceDiscoObserver& aObserver)
    {
    CServiceDiscoverer* self = new (ELeave) CServiceDiscoverer(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }


void CServiceDiscoverer::ConstructL()
    {
    iRunning = EFalse;
    }

CServiceDiscoverer::CServiceDiscoverer(MServiceDiscoObserver& aObserver):
    iObserver(aObserver)
    {
    }

CServiceDiscoverer::~CServiceDiscoverer()
    {
    FinishDiscovery();
    }


// ----------------------------------------------------------------------------
// CServiceDiscoverer::DiscoverServicesOnDeviceL(TDeviceData *aDevData)
//
// discover services on given device.  a service discovery agent will be
// started to do the discovery of services on given remote device.
// service discovery will be limited to search only for services with
// our service id.
// ----------------------------------------------------------------------------
void CServiceDiscoverer::DiscoverServicesOnDeviceL(TDeviceData* aDevData)
    {
    FinishDiscovery();
    
    iDevDataChanged=EFalse;
    iDevData=aDevData;

    // init new service discovery agent
    iAgent = CSdpAgent::NewL( *this, iDevData->iDeviceAddr );
    // set search properties for agent
    iSpat = CSdpSearchPattern::NewL();
    // use our service id to filter the services discovered
    // -> will return only the services with matching service id(s)
    TUUID serviceUUID(KBT_serviceID);
    iSpat->AddL(serviceUUID);
    iAgent->SetRecordFilterL(*iSpat);

    // initiate search
    // this will result in call to NextRecordRequestComplete()
    iAgent->NextRecordRequestL();
    iRunning = ETrue;
    }


// ----------------------------------------------------------------------------
// CServiceDiscoverer::DiscoverServicesL(TDeviceDataList* aDevDataList)
//
// discover services of all devices on the given device data list.  this will
// make the initial call to DiscoverServicesOnDeviceL(), the further calls
// to DiscoverServicesOnDeviceL() will be made by NextRecordRequestComplete()
// as the pending service discovery request completes.
// ----------------------------------------------------------------------------
void CServiceDiscoverer::DiscoverServicesL(TDeviceDataList* aDevDataList)
    {
    if ( aDevDataList->Count()> 0 )
        {
        iDeviceIdx=0;
        iDevDataList=aDevDataList;
        DiscoverServicesOnDeviceL((*iDevDataList)[iDeviceIdx]);
        }
    }


// ----------------------------------------------------------------------------
// CServiceDiscoverer::FinishDiscovery()
//
// stop discovering services, stop service discovery agent.
// ----------------------------------------------------------------------------
void CServiceDiscoverer::FinishDiscovery()
    {
    if(iAgent)
        iAgent->Cancel();
    delete iAgent;
    iAgent=NULL;
    if(iSpat)
        iSpat->Reset();
    delete iSpat;
    iSpat=NULL;
    }


// ----------------------------------------------------------------------------
// CServiceDiscoverer::NextRecordRequestComplete(
//      TInt aError,
//      TSdpServRecordHandle aHandle,
//      TInt aTotalRecordsCount)
//
// called when the service discovery agent has completed discovering services
// on device.  now the attributes of the found service records (if any) must
// be evaluated.  if no service records were discovered, proceed doing
// service discovery on next device.
// ----------------------------------------------------------------------------
void CServiceDiscoverer::NextRecordRequestComplete(
    TInt aError,
    TSdpServRecordHandle aHandle,
    TInt aTotalRecordsCount)
    {
    iRunning = EFalse;
    
    if ( aError==KErrNone && aTotalRecordsCount>0 )
        {
        // we got records, retrieve attributes for record
        // request protocol descriptor from remote device records,
        // we need this to retrieve remote port to connect to later on..
        TRAPD(err,iAgent->AttributeRequestL(aHandle, KSdpAttrIdProtocolDescriptorList) );
        if( err )
            TRAP(err,iObserver.ReportServiceDiscoveryErrorL(err));
        }
    else
        {
        // done with this device, store data if changed
        if ( iDevDataChanged )
            {
            iDevData->iDeviceServicePort=iPort;
            (*iDevDataList)[iDeviceIdx]=iDevData;
            }

        // discover services on next device, if any left
        iDeviceIdx++;
        if ( iDeviceIdx<iDevDataList->Count() )
            {
            // more devices to probe, proceed
            TRAPD(err,DiscoverServicesOnDeviceL((*iDevDataList)[iDeviceIdx]));
            if( err )
                TRAP(err,iObserver.ReportServiceDiscoveryErrorL(err))
            }
        else
            {
            FinishDiscovery();
            // all devices done, notify
            TRAPD(err,iObserver.HandleServiceDiscoveryCompleteL());
            }
        }
    }


// ----------------------------------------------------------------------------
// CServiceDiscoverer::AttributeRequestResult(
//      TSdpServRecordHandle /*aHandle*/,
//      TSdpAttributeID /*aAttrID*/,
//      CSdpAttrValue* aAttrValue)
//
// called when the service attributes for the service record have been
// retrieved.
// ----------------------------------------------------------------------------
void CServiceDiscoverer::AttributeRequestResult(
    TSdpServRecordHandle /*aHandle*/,
    TSdpAttributeID /*aAttrID*/,
    CSdpAttrValue* aAttrValue)
    {
    // parse attributes, will result in call to VisitAttributeValue()
    TRAPD(err,aAttrValue->AcceptVisitorL(*this) );
    if( err )
        TRAP(err,iObserver.ReportServiceDiscoveryErrorL(err));

    delete aAttrValue;
    }


// ----------------------------------------------------------------------------
// CServiceDiscoverer::AttributeRequestComplete(
//      TSdpServRecordHandle /*aHandle*/,
//      TInt aError)
//
// called when the request to resolve the service attributes for the service
// record completes.  if there are more service records, proceed resolving
// the next service record.
// ----------------------------------------------------------------------------
void CServiceDiscoverer::AttributeRequestComplete(
    TSdpServRecordHandle /*aHandle*/,
    TInt aError)
    {
    if ( aError==KErrNone )
        {
        // done with attributes for this record, request next
        // service record
        TRAPD(err,iAgent->NextRecordRequestL());
        if( err )
            TRAP(err,iObserver.ReportServiceDiscoveryErrorL(err));
        }
    else
        {
        // error, should terminate discoverer?
        }
    }


// ----------------------------------------------------------------------------
// CServiceDiscoverer::VisitAttributeValueL(
//      CSdpAttrValue &aValue,
//      TSdpElementType aType)
//
// called for processing of each service attribute.  here we must look for
// attributes of UUID type.  if the UUID is RFCOMM UUID, resolve the value
// for this attribute, which will be channel number to be used for connection
// to remote device.
// ----------------------------------------------------------------------------
void CServiceDiscoverer::VisitAttributeValueL(
    CSdpAttrValue &aValue,
    TSdpElementType aType)
    {
    switch (aType)
        {
        case ETypeUUID:
            {
            TPtrC8 uuid(aValue.UUID().ShortestForm());
            iLastUUID.SetL(uuid);
            break;
            }

        case ETypeUint:
            {
            if ( iLastUUID==KRFCOMM )
            {
                // previous call to this method with rfcomm UUID, therefore
                // this one will be the value, rfcomm service channel (port)
                iPort=aValue.Uint();
                // mark device data changed, so the device data record in
                // device data list will be updated.
                iDevDataChanged=ETrue;
            }
            break;
            }

        default:
            // rest don't really matter..
            break;

        }
    }


void CServiceDiscoverer::StartListL(CSdpAttrValueList& /*aList*/)
    {
    // not needed
    }

void CServiceDiscoverer::EndListL()
    {
    // not needed
    }


// ----------------------------------------------------------------------------
// CServiceDiscoverer::HasServices()
//
// returns true if any services matching our service id were found on any
// remote device.
// ----------------------------------------------------------------------------
TBool CServiceDiscoverer::HasServices()
    {
    TBool exists = EFalse;
    if (iDevDataList)
        {
        if (iDevDataList->Count() > 0)
            {
            exists = ETrue;
            }
        }
    return exists;
    }


