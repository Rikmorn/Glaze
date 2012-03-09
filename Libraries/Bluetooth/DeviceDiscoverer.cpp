/*
 * Copyright © 2009 Nokia Corporation.
 */
// INCLUDE FILES
#include "DeviceDiscoverer.h"
_LIT(KBTLinkManagerTxt,"BTLinkManager");

CDeviceDiscoverer* CDeviceDiscoverer::NewL(RSocketServ& aSocketServ,
		MDeviceDiscoObserver& aObserver) {
	CDeviceDiscoverer* self = CDeviceDiscoverer::NewLC(aSocketServ, aObserver);
	CleanupStack::Pop(self);
	return self;
}

CDeviceDiscoverer* CDeviceDiscoverer::NewLC(RSocketServ& aSocketServ,
		MDeviceDiscoObserver& aObserver) {
	CDeviceDiscoverer* self = new (ELeave) CDeviceDiscoverer(aSocketServ, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

void CDeviceDiscoverer::ConstructL() {
}

CDeviceDiscoverer::CDeviceDiscoverer(RSocketServ& aSocketServ,
		MDeviceDiscoObserver& aObserver) :
	CActive(CActive::EPriorityStandard), iSocketServ(aSocketServ), iObserver(
			aObserver) {
	CActiveScheduler::Add(this);
}

CDeviceDiscoverer::~CDeviceDiscoverer() {
	// cancel active object
	Cancel();
	iResolver.Close();
}

// ----------------------------------------------------------------------------
// CDeviceDiscoverer::DiscoverDevicesL(TDeviceDataList *aDevDataList)
//
// discover remote devices.  RHostResolver will be used to do the discovery.
// any found devices will be placed in aDevDataList.
// ----------------------------------------------------------------------------
void CDeviceDiscoverer::DiscoverDevicesL(TDeviceDataList* aDevDataList) {
	if (!IsActive()) {
		// initialize host resolver
		// load protocol for discovery
		TProtocolDesc pdesc;
		User::LeaveIfError(iSocketServ.FindProtocol(KBTLinkManagerTxt(), pdesc));

		iResolver.Close();
		User::LeaveIfError(iResolver.Open(iSocketServ, pdesc.iAddrFamily,
				pdesc.iProtocol));

		// wipe existing device data list, start fresh
		iDevDataList = aDevDataList;
		iDevDataList->Reset();

		// start device discovery by invoking remote address lookup
#ifdef ENABLE_LIAC
		TUint myIAC( iLIAC?KLIAC:KGIAC );

		iAddr.SetIAC( myIAC );
#else
		iAddr.SetIAC(KGIAC);
#endif

		iAddr.SetAction(KHostResInquiry | KHostResName | KHostResIgnoreCache);
		iResolver.GetByAddress(iAddr, iEntry, iStatus);
		SetActive();
	} else {
		User::Leave(KErrNotReady);
	}
}

void CDeviceDiscoverer::RunL() {
	// RHostResolver.GetByAddress(..) has completed, process results
	if (iStatus != KErrNone) {
		// all devices (if any) done, notify 
		if (iDevDataList->Count() > 0) {
			iObserver.HandleDeviceDiscoveryComplete(KErrNone);
		} else {
			iObserver.HandleDeviceDiscoveryComplete(KErrNotFound);
		}
	} else {
		// new device data entry
		TDeviceData *devData = new (ELeave) TDeviceData();
		devData->iDeviceName = iEntry().iName;
		devData->iDeviceAddr
				= static_cast<TBTSockAddr> (iEntry().iAddr).BTAddr();
		devData->iDeviceServicePort = 0;
		// add device data entry
		iDevDataList->Append(devData);

		iObserver.DeviceDiscovered(*devData);

		// get next discovered device
		iResolver.Next(iEntry, iStatus);
		// wait for resolver to complete
		SetActive();
	}
}

void CDeviceDiscoverer::DoCancel() {
	iResolver.Cancel();
}

// ----------------------------------------------------------------------------
// CDeviceDiscoverer::HasDevices()
//
// returns true if any devices were discovered
// ----------------------------------------------------------------------------
TBool CDeviceDiscoverer::HasDevices() {
	TBool exists = EFalse;

	if (iDevDataList) {
		if (iDevDataList->Count() > 0) {
			exists = ETrue;
		}
	}
	return exists;
}

// ----------------------------------------------------------------------------
// CBluetoothPMPExampleEngine::SetLIAC
// ----------------------------------------------------------------------------
#ifdef ENABLE_LIAC
void CDeviceDiscoverer::SetLIAC( TBool aState )
{
	iLIAC = aState;
}
#endif

// ----------------------------------------------------------------------------
// CBluetoothPMPExampleEngine::StopDiscovery
// ----------------------------------------------------------------------------
void CDeviceDiscoverer::StopDiscovery() {
	Cancel();

	if (HasDevices()) {
		iObserver.HandleDeviceDiscoveryComplete(KErrNone);
	} else {
		iObserver.HandleDeviceDiscoveryComplete(KErrNotFound);
	}
}
