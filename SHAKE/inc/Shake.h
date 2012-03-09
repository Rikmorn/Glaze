/*
============================================================================
 Name        : Shake.h
 Author      : Roberto Sousa
 Version     : 0.1
 Copyright   : 
 Description : Shake.h - CShake class header, defines the entry point for
				dll. Add any methods or classes needed to read the raw data
============================================================================
*/

#ifndef __SHAKEDLL_H__
#define __SHAKEDLL_H__

#include "IInput.h"
#include "BluetoothEngine.h"
#include "ShakeParser.h"

#include <e32base.h>    // CBase
#include <e32std.h>
#include <e32math.h>

_LIT(KName, "SHAKE");

class CShake : public IInput, MBluetoothObserver {
public:
	IMPORT_C static IInput* NewL();
	CShake();

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
	
	void activateShake();
	
private:
	CBluetoothEngine *iBluetooth;
	RArray<TInt>* iConnect; 
	CShakeParser* iParser;
};

#endif  // __SHAKEDLL_H__


