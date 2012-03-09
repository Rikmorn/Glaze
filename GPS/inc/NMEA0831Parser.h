/*
 * NMEA0831Parser.h
 *
 *  Created on: 17/Abr/2009
 *      Author: Daemoniorum
 */

#ifndef NMEA0831PARSER_H_
#define NMEA0831PARSER_H_

#include <e32std.h>
#include <e32base.h>

// some strings to lookout for in the nmea stream
_LIT(KGGA,"GPGGA");
_LIT(KGSA,"GPGSA");
_LIT(KGSV,"GPGSV");
_LIT(KRMC,"GPRMC");

// number os samples in the rolling avrerage filter
#define NUMSAMPLES 5

class CNMEA{
public:
	static CNMEA* NewL();
	virtual ~CNMEA();

protected: 
	CNMEA();
    void ConstructL( void );
    
public:
	void Parse(const TDesC& aData);
	
private:
	void handleToken();
	TInt currentToken(const TDesC& aData);
	void parseToken();
	void addCoord(TInt, TReal64);
	
public:
	TBuf<50> iBuffer;
	TBuf<2> TimeHours;
	TBuf<2> TimeMinutes;
	TBuf<2> TimeSeconds;
	TChar Status;
	TReal64 Latitude;
	TReal64 Longitude;
	TReal64 Altitude;
	TInt Fix;
	TInt SateliteNumberView;
	
private:
	TReal64 tempLat;
	TReal64 tempLong;
	TReal64 tempAlt;
	
	TReal64 iLatBuffer[NUMSAMPLES];
	TReal64 iLongBuffer[NUMSAMPLES];
	TReal64 iAltBuffer[NUMSAMPLES];
	TInt iLBindex, iLGBindex, iABindex;
	
	TInt iTokenNum;
	TBool iDolarFound;
	TInt iCommand;
	TInt iPosition;
};

#endif /* NMEA0831PARSER_H_ */
