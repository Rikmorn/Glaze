/*
 * Copyright © 2009 Nokia Corporation.
 */

#ifndef SERVICEDISCOVERER_H
#define SERVICEDISCOVERER_H

// INCLUDES
#include <e32base.h>
#include <aknviewappui.h>

#include <coecntrl.h>
#include <es_sock.h>
#include <btdevice.h>
#include <bt_sock.h>
#include <btsdp.h>

#include "Common.h"

class MServiceDiscoObserver
{

public:
/*!
 * HandleServiceDiscoveryCompleteL()
 *
 * discussion Observer interface for handling service discovery complete event.
 */
    virtual void HandleServiceDiscoveryCompleteL()=0;
    virtual void ReportServiceDiscoveryErrorL(TInt aError)=0;
};


class CServiceDiscoverer : public CBase,
                           public MSdpAgentNotifier,
                           public MSdpAttributeValueVisitor
{
public:

/*!
 * NewL()
 *
 * discussion Create new CServiceAdvertiser object
 * return a pointer to the created instance of CServiceAdvertiser
 */
    static CServiceDiscoverer* NewL(MServiceDiscoObserver& aObserver);

/*!
 * NewLC()
 *
 */
    static CServiceDiscoverer* NewLC(MServiceDiscoObserver& aObserver);

/*!
 * ~CServiceDiscoverer()
 *
 * discussion Destroy the object and release all memory objects
 */
    ~CServiceDiscoverer();

/*!
 * DiscoverServiceOnDeviceL()
 *
 * discussion Discovers services on a given device.  service discovery
 * agent will be started to do the service discovery.  search pattern
 * will be used to limit the discovery for our services (matching service
 * id).
 *
 * param aDevData device data record
 */
    void DiscoverServicesOnDeviceL(TDeviceData* aDevData);

/*!
 * DiscoverServiceOnDeviceL()
 *
 * discussion Starts service discovery for all given devices.
 *
 * param aDevDataList list of devices to do the service discovery for.
 */
    void DiscoverServicesL(TDeviceDataList* aDevDataList);

/*!
 * FinishDiscovery()
 *
 * discussion Stops the service discovery, stops the discovery agent.
 *
 * param aDevData device data record
 */
    void FinishDiscovery();

/*!
 * HasServices()
 *
 * discussion check if any services were discovered.
 *
 * return true if any usable services were discovered.
 */
    TBool HasServices();


private:

/*!
 * CServiceDiscoverer()
 *
 * discussion Perform the first phase of two phase construction
 */
    CServiceDiscoverer(MServiceDiscoObserver& aObserver);

/*!
 * ConstructL()
 *
 */
    void ConstructL();

/*!
 * HandleServiceDiscoveryCompleteL()
 *
 * discussion Handles the service discovery completed event.  the handling
 * request will be passed onto handling observer.
 */
    void HandleServiceDiscoveryCompleteL();

/*
 * NextRecordRequestComplete()
 *
 * discussion called when an service record
 *            (CSdpAgent::NextRecordRequestL()) operation completes.
 * param aError KErrNone, or an SDP error
 * param aHandle service record for which the query was made
 * param aTotalRecordsCount total number of matching records
 */
    void NextRecordRequestComplete(
        TInt aError,
        TSdpServRecordHandle aHandle,
        TInt aTotalRecordsCount);

/*
 * AttributeRequestResult()
 *
 * discussion Called by the attribute request
 *            (CSdpAgent::AttributeRequestL()) to pass the results of
 *            a successful attribute request.
 *
 * param aHandle service record for which the query was made
 * param aAttrID ID of the attribute obtained
 * param aAttrValue attribute value obtained
 */
    void AttributeRequestResult(
        TSdpServRecordHandle aHandle,
        TSdpAttributeID aAttrID,
        CSdpAttrValue* aAttrValue);

/*!
 * AttributeRequestComplete()
 *
 * discussion Called when the attribute request
 *            (CSdpAgent::AttributeRequestL()) completes.
 *
 * param aHandle service record for which the query was made
 * param aError KErrNone, or an SDP error
 */
    void AttributeRequestComplete(
        TSdpServRecordHandle aHandle,
        TInt aError);

/*!
 * VisitAttributeValueL()
 *
 * discussion Called to pass the attribute request values
 *
 * param aValue attribute value
 * param aType attribute type
 */
    void VisitAttributeValueL(CSdpAttrValue& aValue, TSdpElementType aType);
    // not needed -> not implemented
    void StartListL(CSdpAttrValueList &aList);
    // not needed -> not implemented
    void EndListL();

public:
    // Service Discovery is running
    TBool iRunning;

private:    // data

    // observer reference
    MServiceDiscoObserver& iObserver;
    // service discovery agent
    CSdpAgent* iAgent;
    // service discovery search pattern
    CSdpSearchPattern* iSpat;
    // last discovered uuid in the service attributes
    TUUID iLastUUID;
    // port (comm channel) found in the service attributes
    TUint iPort;
    // device data record reference
    TDeviceData* iDevData;
    // device data list reference
    TDeviceDataList* iDevDataList;
    // this indicates that a port number was found in the service attributes
    // and device data record for this device needs to be updated with the
    // port number.
    TBool iDevDataChanged;
    // index of device in device data list used for service discovery
    TInt iDeviceIdx;
};

#endif
