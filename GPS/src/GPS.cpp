/*
 ============================================================================
 Name        : CGPSDll from GPSDll.h
 Author      : Roberto Sousa
 Version     : 0.1
 Copyright   : 
 Description : CGPS DLL source
 ============================================================================
 */

//  Include Files  

#include "GPS.h"			// CGPS
#include "GPS.pan"      	// panic codes
// DLL constructor, no more can be added to the method
EXPORT_C IInput* CGPS::NewL() {
	return new CGPS();
}

/* CGPS constructor */
CGPS::CGPS() {
	iBluetooth = CBluetoothEngine::NewL(*this);
	iBluetooth->TurnBtOnL();
	iBluetooth->DiscoverDevicesL();
	iParser = CNMEA::NewL();
	iConnect = new (ELeave) RArray<TInt> ;
}

/* 
 Query method. the objective of this method is to receive 
 a query from an exterior component analyse that query
 and provide a responce, that response must be saved in
 out.
 */
void CGPS::query(TAny* out, const TDesC& query) {
	((RArray<TReal64>*) out)->Reset();
	((RArray<TReal64>*) out)->AppendL(iParser->Latitude);
	((RArray<TReal64>*) out)->AppendL(iParser->Longitude);
	((RArray<TReal64>*) out)->AppendL(iParser->Altitude);
}

/*
 Cleanup method this function will be called right before 
 the deletion of Input, so this method can be seen as the
 destructor of the DLL.
 */
void CGPS::cleanup() {
	delete iBluetooth;
	delete iParser;
	iConnect->Close();
	delete iConnect;
}

/* bluetooth related methods*/
void CGPS::HandleListenerDataReceivedL(const TDesC& aData) {

}

void CGPS::HandleListenerConnectedL() {

}

void CGPS::HandleListenerDisconnectedL() {

}

void CGPS::HandleConnectorDataReceivedL(THostName aName, const TDesC& aData) {
	// BT gps supports GGA, GSA, GSV, RMC
	// NMEA 0183 specs http://gpsd.berlios.de/NMEA.txt
	iParser->Parse(aData);
}

void CGPS::HandleDeviceDiscoveryComplete(TInt aError) {
	// display devices with service we can use
	for (TInt idx = 0; idx < (iBluetooth->iDevDataList.Count()); idx++) {
		TDeviceData *dev = iBluetooth->iDevDataList[idx];
		if (dev->iDeviceName.Find(_L("GPS")) > 0)
			iConnect->Append(idx);
	}
	if (iConnect->Count() > 0)
		iBluetooth->DiscoverServicesL();
}

void CGPS::HandleServiceDiscoveryCompleteL() {
	iBluetooth->ConnectDevicesL(iConnect); // connect to devices
}

void CGPS::DeviceDiscovered(const TDeviceData& aDevice) {
	//iBluetooth->DiscoverServicesL();
}

void CGPS::ServiceDiscoveryErrorL(TInt aError) {

}
