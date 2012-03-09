/*
 * NMEA0831Parser.cpp
 *
 *  Created on: 17/Abr/2009
 *      Author: Daemoniorum
 */

#include "NMEA0831Parser.h"

CNMEA* CNMEA::NewL() {
	CNMEA* self = new (ELeave) CNMEA();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

CNMEA::CNMEA() {
	iCommand = 0;
	iDolarFound = EFalse;
	iTokenNum = -1;
	iBuffer.Zero();
	TimeHours.Zero();
	TimeMinutes.Zero();
	TimeSeconds.Zero();
	Latitude = 0;
	Longitude = 0;
	Altitude = 0;
	iLBindex = 0;
	iLGBindex = 0;
	iABindex = 0;
}

CNMEA::~CNMEA() {
}

void CNMEA::ConstructL(void) {
}

// parse the stream directly
void CNMEA::Parse(const TDesC& aData) {
	TInt size = aData.Length();
	for (iPosition = 0; /*aData[iPosition] != '\n'*/iPosition < size; iPosition++) {
		if (aData[iPosition] == '$' || aData[iPosition] == ',') {
			if (aData[iPosition] == '$') {
				//iBuffer.Zero();
				iDolarFound = ETrue;
			} else
				handleToken();
		} else
			iBuffer.Append(aData[iPosition]);
	}
}

//handle tokens if found
void CNMEA::handleToken() {
	if (iBuffer.Length() == 0)
		return;
	if (iDolarFound) {
		iTokenNum = 0;
		iCommand = currentToken(iBuffer);
		iDolarFound = EFalse;
	} else {
		parseToken();
		iTokenNum++;
	}
	iBuffer.Zero();
}

// assign a number to each token
TInt CNMEA::currentToken(const TDesC& aData) {
	TInt p = aData.Find(KGGA);
	if (p != KErrNotFound)
		return 0;
	p = aData.Find(KGSA);
	if (p != KErrNotFound)
		return 1;
	p = aData.Find(KGSV);
	if (p != KErrNotFound)
		return 2;
	p = aData.Find(KRMC);
	if (p != KErrNotFound)
		return 3;
	return -1; // non of the above
}

// parse the token according to iCommand and iTokenNum
void CNMEA::parseToken(){
	if (iBuffer.Length() == 0)
		return;
	if (iCommand == 0) {
		if (iTokenNum == 0) {
			TimeHours.Zero();
			TimeHours.Append(iBuffer[0]);
			TimeHours.Append(iBuffer[1]);
			TimeMinutes.Zero();
			TimeMinutes.Append(iBuffer[2]);
			TimeMinutes.Append(iBuffer[3]);
			TimeSeconds.Zero();
			TimeSeconds.Append(iBuffer[4]);
			TimeSeconds.Append(iBuffer[5]);
		} else if (iTokenNum == 1) {
			TBuf<2> d;
			TInt deg;
			d.Append(iBuffer[0]);
			d.Append(iBuffer[1]);
			TLex iLex(d);
			iLex.Val(deg);

			TReal64 time;
			iBuffer.Delete(0, 2);
			iLex.Assign(iBuffer);
			iLex.Val(time);

			tempLat = deg + (time / 60);
			
		} else if (iTokenNum == 2) {
			//Latitude = (iBuffer[0] == 'S') ? -tempLat : tempLat;
			addCoord(0, (iBuffer[0] == 'S') ? -tempLat : tempLat);
		} else if (iTokenNum == 3) {
			TBuf<3> d;
			TInt deg;
			d.Append(iBuffer[0]);
			d.Append(iBuffer[1]);
			d.Append(iBuffer[2]);
			TLex iLex(d);
			iLex.Val(deg);

			TReal64 time;
			iBuffer.Delete(0, 3);
			iLex.Assign(iBuffer);
			iLex.Val(time);

			tempLong = deg + (time / 60);
		} else if (iTokenNum == 4) {
			//Longitude = (iBuffer[0] == 'W') ? -tempLong : tempLong;
			addCoord(1, (iBuffer[0] == 'W') ? -tempLong : tempLong);
		} else if (iTokenNum == 5) {
			TLex iLex(iBuffer);
			iLex.Val(Fix);
		} else if (iTokenNum == 6) {
			TLex iLex(iBuffer);
			iLex.Val(SateliteNumberView);
		} else if (iTokenNum == 10) {
			TLex iLex(iBuffer);
			iLex.Val(tempAlt);
			addCoord(2, tempAlt);
		}
	}
	if (iCommand == 3 && iTokenNum == 1)
			Status = iBuffer[0];
}

/*
 * calculates the rolling average as it receives values
 * aType identifies which coordenate is being sent
 * 0 - Latitude
 * 1 - Longitude
 * 2 - Altitude
 */
 
void CNMEA::addCoord(TInt aType, TReal64 aValue) {
	TInt i;
	TReal64 temp = 0;
	switch (aType) {
	case 0:
		iLatBuffer[iLBindex % NUMSAMPLES] = aValue;
		if (iLBindex >= NUMSAMPLES - 1) {
			for (i = 0; i < NUMSAMPLES; i++) {
				temp += iLatBuffer[i];
			}
			Latitude = temp / NUMSAMPLES;
		}
		iLBindex++;
		break;
	case 1:
		iLongBuffer[iLGBindex % NUMSAMPLES] = aValue;
		if (iLGBindex >= NUMSAMPLES - 1) {
			for (i = 0; i < NUMSAMPLES; i++) {
				temp += iLongBuffer[i];
			}
			Longitude = temp / NUMSAMPLES;
		}
		iLGBindex++;
		break;
	case 2:
		iAltBuffer[iABindex % NUMSAMPLES] = aValue;
		if (iABindex >= NUMSAMPLES - 1) {
			for (i = 0; i < NUMSAMPLES; i++) {
				temp += iAltBuffer[i];
			}
			Altitude = temp / NUMSAMPLES;
		}
		iABindex++;
		break;
	}
}
