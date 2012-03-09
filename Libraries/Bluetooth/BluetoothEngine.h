/*
 * Copyright © 2009 Nokia Corporation.
 */

#ifndef BLUETOOTHENGINE_H
#define BLUETOOTHENGINE_H

// INCLUDES
#include "Common.h"
#include "DeviceDiscoverer.h"
#include "ServiceDiscoverer.h"
#include "ServiceAdvertiser.h"
#include "Listener.h"
#include "Connector.h"

#include <e32base.h>
#include <coecntrl.h>
#include <es_sock.h>
#include <btdevice.h>
#include <bt_sock.h>
#include <btsdp.h>
#include <btmanclient.h>

#ifdef ENABLE_LIAC
#include <e32property.h>
#endif

// maximum number of bluetooth device connections
const TInt KMaxConnectedDevices = 7;

// length of label message buffer
const TInt KMsgLength = 384;

//avoiding magic numbers ;)
const TInt KThirty = 30;
const TInt KTwenty = 20;
const TInt KEighty = 80;
const TInt KTwelve = 12;
const TInt KForty = 40;

// CLASS DECLARATION

class MBluetoothObserver {
public:
	virtual void HandleListenerDataReceivedL(const TDesC& aData)=0;
	virtual void HandleListenerConnectedL()=0;
	virtual void HandleListenerDisconnectedL()=0;
	virtual void HandleConnectorDataReceivedL(THostName aName, const TDesC& aData)=0;
	virtual void HandleDeviceDiscoveryComplete(TInt aError) = 0;
	virtual void HandleServiceDiscoveryCompleteL()=0;	
	virtual void DeviceDiscovered(const TDeviceData& aDevice)=0;
	virtual void ServiceDiscoveryErrorL(TInt aError)=0;
};


class CBluetoothEngine: public CBase,
		public MListenerObserver,
		public MConnectorObserver,
		public MDeviceDiscoObserver,
		public MServiceDiscoObserver {
public:

	static CBluetoothEngine* NewL(MBluetoothObserver& aObserver);

	static CBluetoothEngine* NewLC(MBluetoothObserver& aObserver);

	~CBluetoothEngine();

public:
	// Discovers bluetooth devices within range.  the discovered
	void DiscoverDevicesL();

	//Starts service discovery for services on remote devices.
	void DiscoverServicesL();

	/*
	 * Starts the application in slave more.  the application will open
	 * listening socket to listen to incoming connection request, and advertise
	 * its services.
	 */
	void StartSlaveL();

	void StopSlaveL();

	//Send a message to all connected slaves.
	void SendMessageL(const TDesC8& aData);

	//Connects to the remote devices that were discovered to offer the service we require.
	void ConnectDevicesL();
	
	void ConnectDevicesL(RArray<TInt>*);

	//Disconnects from connected remote devices.
	void DisconnectDevices();

	//Displays the connected remote devices.
	//void ShowConnectedDevicesL();

	//Handles the data slave received from a master.
	void HandleListenerDataReceivedL(const TDesC& aData);

	/*
	 * Handles the data a master received from a slave.
	 * param aName the name of the slave that sent the data.
	 * param aData the data received.
	 */
	void HandleConnectorDataReceivedL(THostName aName, const TDesC& aData);
	void HandleConnectorErrorL(THostName aName, TInt aError);
	
	/*
	 * Handles the event of slave connection (slave was connected to
	 * by master).
	 */
	void HandleListenerConnectedL();

	/*
	 * Handles the event of slave disconnection, the master connection
	 * to slave was terminated.
	 */
	void HandleListenerDisconnectedL();

	//Handles the event of device discovery completion.
	void HandleDeviceDiscoveryComplete(TInt aError);
	
	void DeviceDiscovered(const TDeviceData &aDevice);

	//Handles the event of service discovery completion.
	void HandleServiceDiscoveryCompleteL();

	//An error has occured during service discovery
	void ReportServiceDiscoveryErrorL(TInt aError);

	//Returns true if master has any slave connections.
	TBool HasConnections();

	//Ask the user to turn on Bluetooth
	void TurnBtOnL();

#ifdef ENABLE_LIAC
	//Set LimitedDiscoverableStatus status
	void SetLIAC( TBool aState );

	//Returns LimitedDiscoverableStatus status
	TBool LIAC();
#endif

	//Returns ETrue if there is an ongoing device discovery
	TBool IsDiscoveryActive();

	//Stops device discovery if there is any ongoing
	void StopDiscovery();

private:
	void ConstructL();
	CBluetoothEngine(MBluetoothObserver& aObserver);

public:
	// listener
	CListener* iListener;
	// device discoverer
	CDeviceDiscoverer* iDeviceDiscoverer;
	// service advertiser
	CServiceAdvertiser* iServiceAdvertiser;
	// service discoverer
	CServiceDiscoverer* iServiceDiscoverer;
	// true if the application is acting as a slave
	TBool iIsSlave;
	// true if the application is acting as a master
	TBool iIsMaster;
	
	// device data list reference
	TDeviceDataList iDevDataList;

private:
	MBluetoothObserver& iObserver;
	// socket server
	RSocketServ iSocketServ;
	// array holding the connectors
	TFixedArray<CConnector*, KMaxConnectedDevices> iConnectedDevices;
	// number of slave connections master has
	TInt iConnectedDeviceCount;

#ifdef ENABLE_LIAC
	//property for LimitedDiscoverableStatus
	RProperty iProperty;

	//flag if LimitedDiscoverableStatus is enabled
	TBool iLIAC;
#endif

};

#endif

// End of File
