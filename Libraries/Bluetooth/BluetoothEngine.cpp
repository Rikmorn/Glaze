/*
 * Copyright © 2009 Nokia Corporation.
 */

// INCLUDE FILES
#include <utf.h>
#include "Common.h"
#include "BluetoothEngine.h"
#include "DeviceDiscoverer.h"
#include "ServiceAdvertiser.h"
#include "ServiceDiscoverer.h"
#include "Listener.h"
#include "Connector.h"
#include <BtPmpEx.rsg>

CBluetoothEngine* CBluetoothEngine::NewL(MBluetoothObserver& aObserver) {
	CBluetoothEngine* self = CBluetoothEngine::NewLC(aObserver);
	CleanupStack::Pop(self);
	return self;
}

CBluetoothEngine* CBluetoothEngine::NewLC(MBluetoothObserver& aObserver) {
	CBluetoothEngine* self = new (ELeave) CBluetoothEngine(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

// Standard EPOC 2nd phase constructor
void CBluetoothEngine::ConstructL() {
	// get socket server session
	User::LeaveIfError(iSocketServ.Connect());

	// init listener
	iListener = CListener::NewL(*this, iSocketServ);
	// init device discoverer
	iDeviceDiscoverer = CDeviceDiscoverer::NewL(iSocketServ, *this);

#ifdef ENABLE_LIAC
	// Set default LIAC to OFF
	iLIAC = ETrue;
	// set current LIAC state
	iDeviceDiscoverer->SetLIAC( iLIAC );
#endif
	// init service advertiser
	iServiceAdvertiser = CServiceAdvertiser::NewL();
	// init service discoverer
	iServiceDiscoverer = CServiceDiscoverer::NewL(*this);

	// clean connections table to begin with
	for (TInt idx = 0; idx < KMaxConnectedDevices; idx++) {
		iConnectedDevices[idx] = NULL;
	}

#ifdef ENABLE_LIAC
	// The key to control whether the local device is in Limited Discoverable 
	// mode The P&S value will contain a boolean: ETrue if in limited discoverable mode,
	// otherwise EFalse
	TInt attErr = iProperty.Define(KPropertyUidBluetoothControlCategory,
			KPropertyKeyBluetoothSetLimitedDiscoverableStatus,RProperty::EInt);
	if (attErr != KErrNone && attErr != KErrAlreadyExists)
	{
		User::Leave(attErr);
	}

	// Set LIAC value
	SetLIAC(iLIAC);
#endif
}

CBluetoothEngine::CBluetoothEngine(MBluetoothObserver& aObserver) :
	iIsSlave(EFalse), iObserver(aObserver) {
	//Nothing to do
}

CBluetoothEngine::~CBluetoothEngine() {
#ifdef ENABLE_LIAC
	SetLIAC(EFalse);
	iProperty.Close();
#endif
	// disconnect all devices and clean up connections table
	DisconnectDevices();
	// stop and kill helpers
	delete iServiceAdvertiser;
	iServiceAdvertiser = NULL;
	delete iListener;
	iListener = NULL;
	iDeviceDiscoverer->Cancel();
	delete iDeviceDiscoverer;
	iDeviceDiscoverer = NULL;
	delete iServiceDiscoverer;
	iServiceDiscoverer = NULL;
	iSocketServ.Close();
	// wipe device data list
	iDevDataList.ResetAndDestroy();
}

// ----------------------------------------------------------------------------
// discover bluetooth devices within range.
// ----------------------------------------------------------------------------
void CBluetoothEngine::DiscoverDevicesL() {
	//iStartTime.HomeTime();
	TRAPD(err,iDeviceDiscoverer->DiscoverDevicesL(&iDevDataList));
	if (err) {
		HandleDeviceDiscoveryComplete(err);
	}
}

// ----------------------------------------------------------------------------
// discover services provided by the discovered devices.
// ----------------------------------------------------------------------------
void CBluetoothEngine::DiscoverServicesL() {
	//iStartTime.HomeTime();
	iServiceDiscoverer->DiscoverServicesL(&iDevDataList);
}

// ----------------------------------------------------------------------------
// set application in slave mode.  the device will be set to listen to
// a bluetooth channel, and advertise its service on the channel.
// ----------------------------------------------------------------------------
void CBluetoothEngine::StartSlaveL() {
	if (iIsSlave)
		return;
	TInt channel;
	iListener->StartListenerL(channel);
	iServiceAdvertiser->StartAdvertiserL(channel);
	iIsSlave = ETrue;
}

// ----------------------------------------------------------------------------
// Stop slave mode
// ----------------------------------------------------------------------------
void CBluetoothEngine::StopSlaveL() {
	if (!iIsSlave)
		return;
	iListener->StopListener();
	iServiceAdvertiser->StopAdvertiserL();
	iIsSlave = EFalse;
}

// ----------------------------------------------------------------------------
// disconnect connected devices and clean up connections table
// ----------------------------------------------------------------------------
void CBluetoothEngine::DisconnectDevices() {
	for (TInt idx = 0; idx < KMaxConnectedDevices; idx++) {
		if (iConnectedDevices[idx]) {
			delete iConnectedDevices[idx];
			iConnectedDevices[idx] = NULL;
		}
	}
	iIsMaster = EFalse;
}

// ----------------------------------------------------------------------------
// attempt to connect on all discovered devices (up to 7)
// ----------------------------------------------------------------------------
void CBluetoothEngine::ConnectDevicesL() {
	// close and delete all existing connections
	DisconnectDevices();

	iIsMaster = ETrue;
	// now attempt to connect
	for (TInt idx = 0; idx < (iDevDataList.Count()); idx++) {
		if (iConnectedDeviceCount >= KMaxConnectedDevices)
			return;

		TDeviceData *dev = iDevDataList[idx];

		// if matching service on remote device was found, the service port
		// is set and will be > 0.  if so, we can attempt to connect.
		if (dev->iDeviceServicePort > 0) {
			CConnector* connector = CConnector::NewLC(*this, iSocketServ);
			if ((connector->ConnectL(dev->iDeviceName, dev->iDeviceAddr,
					dev->iDeviceServicePort)) == KErrNone) {
				iConnectedDevices[iConnectedDeviceCount] = connector;
				iConnectedDeviceCount++;
				CleanupStack::Pop(connector);
			} else {
				// connection to device failed!!
				CleanupStack::PopAndDestroy();
			}
		}
	}

}

// ----------------------------------------------------------------------------
// attempt to connect to a group of discovered devices (up to 7)
// ----------------------------------------------------------------------------
void CBluetoothEngine::ConnectDevicesL(RArray<TInt>* aDevices) {
	// close and delete all existing connections
	DisconnectDevices();

	iIsMaster = ETrue;
	// now attempt to connect
	for (TInt idx = 0; idx < (aDevices->Count()); idx++) {
		if (iConnectedDeviceCount >= KMaxConnectedDevices)
			return;

		//TDeviceData *dev = iDevDataList[idx];
		TDeviceData *dev = iDevDataList[(*aDevices)[idx]];

		// if matching service on remote device was found, the service port
		// is set and will be > 0.  if so, we can attempt to connect.
		if (dev->iDeviceServicePort > 0) {
			CConnector* connector = CConnector::NewLC(*this, iSocketServ);
			if ((connector->ConnectL(dev->iDeviceName, dev->iDeviceAddr,
					dev->iDeviceServicePort)) == KErrNone) {
				iConnectedDevices[iConnectedDeviceCount] = connector;
				iConnectedDeviceCount++;
				CleanupStack::Pop(connector);
			} else {
				// connection to device failed!!
				CleanupStack::PopAndDestroy();
			}
		}
	}

}

/*
// ----------------------------------------------------------------------------
// display the devices we are connected to
// ----------------------------------------------------------------------------
void CBluetoothEngine::ShowConnectedDevicesL() {
	TInt count = 0;
	//ShowMessageL(KConnDevicesTxt, ETrue);
	for (TInt idx = 0; idx < KMaxConnectedDevices; idx++) {
		if (iConnectedDevices[idx]) {
			THostName name;
			name = iConnectedDevices[idx]->iName;
			name.Append(KNewLine);
			//ShowMessageL(name, EFalse);
			count++;
		}
	}
	if (count == 0) {
		// no connections!
		// this may be because of no devices has been discovered,
		// or no devices are offering the requested service.
		//ShowMessageL(KNoConns, EFalse);
	}
}*/

// ----------------------------------------------------------------------------
// send a message to all connected devices.
// ----------------------------------------------------------------------------
void CBluetoothEngine::SendMessageL(const TDesC8& aData) {
	if (iIsSlave) {
		iListener->SendData(aData);
	} else {
		// master sending data
		for (TInt idx = 0; idx < KMaxConnectedDevices; idx++) {
			if (iConnectedDevices[idx]) {
				iConnectedDevices[idx]->SendData(aData);
			}
		}
	}
}

// ----------------------------------------------------------------------------
// display data the slave listener receives from the master.  this is a
// callback that CListener class will invoke when it receives new data.
// ----------------------------------------------------------------------------
void CBluetoothEngine::HandleListenerDataReceivedL(const TDesC& aData) {
	iObserver.HandleListenerDataReceivedL(aData);
}

// ----------------------------------------------------------------------------
// a callback received from CListener to indicate that it has been connected to
// ----------------------------------------------------------------------------
void CBluetoothEngine::HandleListenerConnectedL() {
	iObserver.HandleListenerConnectedL();
}

// ----------------------------------------------------------------------------
// a callback received from CListener to indicate that it has been disconnected
// ----------------------------------------------------------------------------
void CBluetoothEngine::HandleListenerDisconnectedL() {
	if (!iIsSlave)
		return;
	StopSlaveL();
	iObserver.HandleListenerDisconnectedL();
	
}

// ----------------------------------------------------------------------------
// display data the master receives from a connected slave.  this is a
// callback that CConnector class will invoke when it receives data from slave.
// Also echo the message to other slaves.
// ----------------------------------------------------------------------------
void CBluetoothEngine::HandleConnectorDataReceivedL(THostName aName,
		const TDesC& aData) {
	iObserver.HandleConnectorDataReceivedL(aName, aData);
	/*
	// display received message
	TBuf<KEighty> msg;
	msg.Format(KFormatStr, &aName, &aData);
	//ShowMessageL(msg, EFalse);
	// echo the message to other slaves
	_LIT8(KSeparator, ":");

	TBuf8<KEighty> buf; //should use HBufC so the size will be big enough
	TPtr8 msgtext8((TUint8*) buf.Ptr(), aData.Size() + KSeparator().Length()
			+ aName.Size());
	CnvUtfConverter::ConvertFromUnicodeToUtf8(msgtext8, aData);

	//convert name to UTF8 so other slaves see
	//the sender name
	TBuf8<KEighty> bufName;
	TPtr8 name8((TUint8*) bufName.Ptr(), aName.Size());
	CnvUtfConverter::ConvertFromUnicodeToUtf8(name8, aName);

	//add the separator and name in the beginning;
	msgtext8.Insert(0, KSeparator);
	msgtext8.Insert(0, name8);

	for (TInt idx = 0; idx < KMaxConnectedDevices; idx++) {
		if (iConnectedDevices[idx]) {
			THostName name;
			name = iConnectedDevices[idx]->iName;

			//echo to other slaves than the sender
			if (name.Compare(aName) != 0)
				iConnectedDevices[idx]->SendData(msgtext8);
		}
	}
	*/
}

// ----------------------------------------------------------------------------
// CBluetoothEngine::HandleConnectorErrorL(THostName aName,TInt aError)
// ----------------------------------------------------------------------------
void CBluetoothEngine::HandleConnectorErrorL(THostName aName, TInt aError) {
	if (aError) {
		// delete errored connection
		for (TInt idx = 0; idx < KMaxConnectedDevices; idx++) {
			if (iConnectedDevices[idx]) {
				THostName name;
				name = iConnectedDevices[idx]->iName;

				//echo to other slaves than the sender
				if (name.Compare(aName) == 0) {
					delete iConnectedDevices[idx];
					iConnectedDevices[idx] = NULL;
				}
			}
		}
		// Is there more connection running?
		iIsMaster = EFalse;
		for (TInt idx = 0; idx < KMaxConnectedDevices; idx++) {
			if (iConnectedDevices[idx]) {
				iIsMaster = ETrue;
				break;
			}
		}
	}
}

// ----------------------------------------------------------------------------
// a callback received from device discoverer to indicate that the device
// discovery has completed.
// ----------------------------------------------------------------------------
void CBluetoothEngine::HandleDeviceDiscoveryComplete(TInt aError) {
	if (aError) {
		iIsMaster = EFalse;
	} else {
		iIsMaster = ETrue;
	}
	iObserver.HandleDeviceDiscoveryComplete(aError);
	/*
	iEndTime.HomeTime();

	TTimeIntervalSeconds seconds;
	iEndTime.SecondsFrom(iStartTime, seconds);

	TInt time = seconds.Int();
	TBuf<KTwelve> temp = KTimeTxt();
	temp.AppendNum(time);
	temp.Append(KSecTxt);
	//TRAPD(err,ShowMessageL(temp,EFalse));

	// iterate and display found devices, if any
	if (iDevDataList.Count() > 0) {
		TBuf<KTwenty> count = KFoundTxt();
		count.AppendNum(iDevDataList.Count());
		count.Append(KDevices);
		//TRAP(err,ShowMessageL(count));
	} else {
		// no devices found
		//TRAP(err,ShowMessageL(KNoDevFound, EFalse));
	}
	*/
}

// ----------------------------------------------------------------------------
// a callback received from service discoverer to indicate that the service
// discovery has completed.
// ----------------------------------------------------------------------------
void CBluetoothEngine::HandleServiceDiscoveryCompleteL() {
	iObserver.HandleServiceDiscoveryCompleteL();
	/*
	iEndTime.HomeTime();

	TTimeIntervalSeconds seconds;
	iEndTime.SecondsFrom(iStartTime, seconds);

	TInt time = seconds.Int();
	TBuf<KTwelve> temp = KTimeTxt();
	temp.AppendNum(time);
	temp.Append(KSecTxt);
	//ShowMessageL(temp,ETrue);

	TInt count = 0;
	// display devices with service we can use
	for (TInt idx = 0; idx < (iDevDataList.Count()); idx++) {
		TDeviceData *dev = iDevDataList[idx];
		if (dev->iDeviceServicePort > 0) {
			THostName name = dev->iDeviceName;
			name.Append(KNewLine);
			//ShowMessageL(name, EFalse);
			count++;
		}
	}
	if (count == 0) {
		//ShowMessageL(KNoServiceFound, EFalse);
	} else {
		//ShowMessageL(KServiceFound, EFalse);
	}
	*/
}

// ----------------------------------------------------------------------------
// returns true if master has any connections to slave(s)
// ----------------------------------------------------------------------------
TBool CBluetoothEngine::HasConnections() {
	TBool exists = EFalse;
	for (TInt idx = 0; idx < KMaxConnectedDevices; idx++) {
		if (iConnectedDevices[idx]) {
			exists = ETrue;
			break;
		}
	}
	return exists;
}

// ----------------------------------------------------------------------------
// a callback to indicate that a device has been found
// ----------------------------------------------------------------------------
void CBluetoothEngine::DeviceDiscovered(const TDeviceData &aDevice) {
	iObserver.DeviceDiscovered(aDevice);
	/*TBuf<KForty> name = aDevice.iDeviceName;
	name.Trim();

	if (name.Length() == 0)
		name.Append(KDeviceWithNoName);

	//TRAPD(err,ShowMessageL(name, EFalse));
	 * */
}

// ----------------------------------------------------------------------------
// CBluetoothEngine::ReportServiceDiscoveryErrorL
// ----------------------------------------------------------------------------
void CBluetoothEngine::ReportServiceDiscoveryErrorL(TInt aError) {
	iObserver.ServiceDiscoveryErrorL(aError);
	//TBuf<KThirty> discError = KServiceDiscoveryError();
	//discError.AppendNum(aError);
	//iAppUi.Container()->ShowMessageL(discError);
}

// ----------------------------------------------------------------------------
//Uses the Notifier API to ask the user to turn on Bluetooth if it's not on already.
// ----------------------------------------------------------------------------
void CBluetoothEngine::TurnBtOnL() {
	//the constant is from btnotifierapi.h which is not in all SDKs
	//so it's hard coded here
	const TUid KPowerModeSettingNotifierUid = { 0x100059E2 };
	//const TUid KBTPowerStateNotifierUid = {0x101F808E}; //S80 and 7710

	RNotifier notifier;
	User::LeaveIfError(notifier.Connect());
	TPckgBuf<TBool> dummy(ETrue);
	TPckgBuf<TBool> reply(EFalse);
	TRequestStatus stat;
	notifier.StartNotifierAndGetResponse(stat, KPowerModeSettingNotifierUid,
			dummy, reply);
	User::WaitForRequest(stat);
	notifier.CancelNotifier(KPowerModeSettingNotifierUid);
	notifier.Close();
}

#ifdef ENABLE_LIAC
// ----------------------------------------------------------------------------
// CBluetoothEngine::SetLIAC
// ----------------------------------------------------------------------------
void CBluetoothEngine::SetLIAC(TBool aState) {
	TInt err = KErrNone;

	// Set LIAC for the system    
	err = iProperty.Set(KPropertyUidBluetoothControlCategory,
			KPropertyKeyBluetoothSetLimitedDiscoverableStatus, aState);

	TBuf<KEighty> myMessage;

	myMessage.Zero();

	if (KErrNone == err) {
		if (aState) {
			myMessage.Append(KLIACOn);
		} else {
			myMessage.Append(KLIACOff);
		}
		iLIAC = aState;

		iDeviceDiscoverer->SetLIAC(iLIAC);

	} else {
		myMessage.AppendFormat(KLIACError, err);
	}

	TRAP( err, ShowMessageL( myMessage, ETrue ));
}

// ----------------------------------------------------------------------------
// CBluetoothEngine::LIAC
// ----------------------------------------------------------------------------
TBool CBluetoothEngine::LIAC() {
	return iLIAC;
}
#endif

// ----------------------------------------------------------------------------
// CBluetoothEngine::IsDiscoveryActive
// ----------------------------------------------------------------------------
TBool CBluetoothEngine::IsDiscoveryActive() {
	return iDeviceDiscoverer->IsActive();
}

// ----------------------------------------------------------------------------
// CBluetoothEngine::StopDiscovery
// ----------------------------------------------------------------------------
void CBluetoothEngine::StopDiscovery() {
	if (IsDiscoveryActive()) {
		iDeviceDiscoverer->StopDiscovery();
	}
	iIsMaster = EFalse;
}
