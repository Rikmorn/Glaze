/*
============================================================================
 Name        : Input.h
 Author      : Roberto Sousa
 Version     : 0.1
 Copyright   : 
 Description : Input.h - CInput class header, defines the entry point for
				dll. Add any methods or classes needed to read the raw data
============================================================================
*/

#ifndef __INPUT_H__
#define __INPUT_H__

#include "IInput.h"
#include "BluetoothEngine.h"
#include "NMEA0831Parser.h"

#include <e32base.h>    // CBase
#include <e32std.h>
#include <e32math.h>

#define e2 0.00669437999014
#define sma 6378137.0

class CGPS : public IInput, MBluetoothObserver {
public:
	IMPORT_C static IInput* NewL();
	CGPS();

	virtual void query(TAny *, const TDesC&);
	virtual void cleanup();
	
private:
	virtual void HandleListenerDataReceivedL(const TDesC& aData);
	virtual void HandleListenerConnectedL();
	virtual void HandleListenerDisconnectedL();
	virtual void HandleConnectorDataReceivedL(THostName aName, const TDesC& aData);
	virtual void HandleDeviceDiscoveryComplete(TInt aError);
	virtual void HandleServiceDiscoveryCompleteL();	
	virtual void DeviceDiscovered(const TDeviceData& aDevice);
	virtual void ServiceDiscoveryErrorL(TInt aError);
	
private:
	CBluetoothEngine *iBluetooth;
	CNMEA* iParser;
	RArray<TInt>* iConnect; 
};

#endif  // __INPUTDLL_H__


