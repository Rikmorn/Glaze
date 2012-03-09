/*
 ============================================================================
 Name        : CShake from Shakel.h
 Author      : Roberto Sousa
 Version     : 0.1
 Copyright   : 
 Description : CShale DLL source
 ============================================================================
 */

//  Include Files  

#include "Shake.h"			// CInput
#include "Shake.pan"      	// panic codes

// DLL constructor, no more can be added to the method
EXPORT_C IInput* CShake::NewL() {
	return new CShake();
}

/* CInput constructor */
CShake::CShake() {
	iParser = CShakeParser::NewL();
	iConnect = new (ELeave) RArray<TInt> ;
	iBluetooth = CBluetoothEngine::NewL(*this);
	iBluetooth->TurnBtOnL();
	iBluetooth->DiscoverDevicesL();
}

/* 
 Query method. the objective of this method is to receive 
 a query from an exterior component analyse that query
 and provide a responce, that response must be saved in
 out.
 */
void CShake::query(TAny* out, const TDesC& query) {
	((RArray<TReal>*) out)->Reset();
	((RArray<TReal>*) out)->Append(iParser->iOrientation / 10.0);
}

/*
 Cleanup method this function will be called right before 
 the deletion of Input, so this method can be seen as the
 destructor of the DLL.
 */
void CShake::cleanup() {
	delete iBluetooth;
	delete iParser;
	iConnect->Close();
	delete iConnect;
}

void CShake::HandleListenerDataReceivedL(const TDesC& aData) {

}

void CShake::HandleListenerConnectedL() {

}

void CShake::HandleListenerDisconnectedL() {

}

void CShake::HandleConnectorDataReceivedL(THostName aName, const TDesC& aData) {
	if (aData.Find(KName) >= 0){
		activateShake();
	} else {
		iParser->Parse(aData);
	}
}

void CShake::HandleDeviceDiscoveryComplete(TInt aError) {
	// display devices with service we can use
	for (TInt idx = 0; idx < (iBluetooth->iDevDataList.Count()); idx++) {
		TDeviceData *dev = iBluetooth->iDevDataList[idx];
		if (dev->iDeviceName.Find(KName) >= 0)
			iConnect->Append(idx);
	}
	if (iConnect->Count() > 0)
		iBluetooth->DiscoverServicesL();
}

void CShake::HandleServiceDiscoveryCompleteL() {
	iBluetooth->ConnectDevicesL(iConnect); // connect to devices
}

void CShake::DeviceDiscovered(const TDeviceData& aDevice) {

}

void CShake::ServiceDiscoveryErrorL(TInt aError) {

}

void CShake::activateShake(){
	// send a little vibration to know it's on.
	TBuf8<50> msgbuffer;
	_LIT8(KPower, "$WRI,0000,1A"); // turn stuff on
	_LIT8(KDataRate,"$WRI,0008,64"); // data rate
	_LIT8(KStreaming,"$WRI,0012,00"); // enable stream
	_LIT8(KCompassFilter,"$WRI,0017,01"); // enable compass filter
	msgbuffer.Copy(KPower);
	iBluetooth->SendMessageL(msgbuffer);
	msgbuffer.Copy(KDataRate);
	iBluetooth->SendMessageL(msgbuffer);
	msgbuffer.Copy(KStreaming);
	iBluetooth->SendMessageL(msgbuffer);
	msgbuffer.Copy(KCompassFilter);
	iBluetooth->SendMessageL(msgbuffer);
	/*
	msgbuffer.Copy(KSomething);
	iBluetooth->SendMessageL(msgbuffer);
	*/
}
