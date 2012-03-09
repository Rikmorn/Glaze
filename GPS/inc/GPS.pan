/*
============================================================================
 Name        : GPS.pan
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Panic codes
============================================================================
*/

#ifndef __GPS_PAN__
#define __GPS_PAN__

/** Input panic codes */
enum TGPSPanic
	{
	EGPSDllNullPointer
	};

inline void Panic(TGPSPanic aReason)
	{
	_LIT(applicationName,"GPS");
	User::Panic(applicationName, aReason);
	}

#endif  // __INPUTDLL_PAN__


