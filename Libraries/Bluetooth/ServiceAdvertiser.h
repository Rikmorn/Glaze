/*
 * Copyright © 2009 Nokia Corporation.
 */

#ifndef SERVICEADVERTISER_H
#define SERVICEADVERTISER_H

// INCLUDES
#include <e32base.h>

#include <coecntrl.h>
#include <es_sock.h>
#include <btdevice.h>
#include <bt_sock.h>
#include <btsdp.h>
#include <btmanclient.h> 

#include "Common.h"

class CServiceAdvertiser: public CBase {
public:

	static CServiceAdvertiser* NewL();

	static CServiceAdvertiser* NewLC();

	~CServiceAdvertiser();

	/*
	 * Starts the service advertiser.
	 * param aChannel the communication channel (port) the service will be
	 * advertised on.  an entry for our advertisement will be added to service
	 * discovery database.
	 */
	void StartAdvertiserL(TInt /*aChannel*/);

	/*!
	 * StopAdvertiserL()
	 *
	 * Stops the service advertiser.  our entry from service 
	 * discovery database will be removed.
	 */
	void StopAdvertiserL();

	/*
	 * Updates the availability of advertised service.  the service
	 * record in service discovery database will be updated accordingly.
	 * aAvailable true if the service should be set as available, 
	 * false if unavailable.
	 */
	void UpdateAvailabilityL(TBool /*aAvailable*/);

private:
	//Perform the first phase of two phase construction
	CServiceAdvertiser();
	
	void ConstructL();

private:
	// service discovery protocol session
	RSdp iSdp;
	// service discovery database (sdp)
	RSdpDatabase iSdpDB;
	// service record
	TSdpServRecordHandle iRecord;
	// service record state
	TInt iRecordState;
};

#endif

