/*
 * Copyright © 2009 Nokia Corporation.
 */

#ifndef DEVICEDISCOVERER_H
#define DEVICEDISCOVERER_H

// INCLUDES
#include <e32base.h>
#include <aknviewappui.h> 

#include <coecntrl.h>
#include <es_sock.h>
#include <btdevice.h>
#include <bt_sock.h>
#include <btsdp.h>

#include "Common.h"

class MDeviceDiscoObserver {

public:
	//Observer interface for handling device discovery complete event.
	virtual void HandleDeviceDiscoveryComplete(TInt aError)=0;
	//nofify observer that a new device was found.
	virtual void DeviceDiscovered(const TDeviceData& aDevice)=0;
};

class CDeviceDiscoverer: public CActive {
public:

	static CDeviceDiscoverer* NewL(RSocketServ& aSocketServ,
			MDeviceDiscoObserver& aObserver);

	static CDeviceDiscoverer* NewLC(RSocketServ& aSocketServ,
			MDeviceDiscoObserver& aObserver);

	~CDeviceDiscoverer();

	//Discovers remote bluetooth devices within range.
	//param aDevDataList device data list where the found devices will be placed.
	void DiscoverDevicesL(TDeviceDataList* aDevDataList);
	//return true if any devices were discovered.
	TBool HasDevices();

#ifdef ENABLE_LIAC
	//Set LimitedDiscoverableStatus status
	void SetLIAC( TBool aState );
#endif

	//Stops ongoing device discovery
	void StopDiscovery();

protected:

	// Handle the active objects request completion.
	void RunL();
	//Cancels the outstanding request on active object.
	void DoCancel();

private:
	//Perform the first phase of two phase construction
	//aSocketServ reference to socket server
	CDeviceDiscoverer(RSocketServ& aSocketServ,
			MDeviceDiscoObserver& aObserver);

	void ConstructL();

private:
	// socket server handle
	RSocketServ& iSocketServ;
	// observer reference
	MDeviceDiscoObserver& iObserver;
	// device data list, to be filled by resolver results
	TDeviceDataList *iDevDataList;
	// host resolver
	RHostResolver iResolver;
	TInquirySockAddr iAddr;
	TNameEntry iEntry;

#ifdef ENABLE_LIAC
	TBool iLIAC;
#endif

};

#endif
