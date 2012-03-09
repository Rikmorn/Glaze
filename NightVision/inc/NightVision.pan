/*
============================================================================
 Name        : NightVision.pan
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Panic codes
============================================================================
*/

#ifndef __NIGHTVISION_PAN__
#define __NIGHTVISION_PAN__

/** Visualization panic codes */
enum TNightvisionPanic
	{
	ENightvisionNullPointer
	};

inline void Panic(TNightvisionPanic aReason)
	{
	_LIT(applicationName,"Nightvision");
	User::Panic(applicationName, aReason);
	}

#endif  // __NIGHTVISION_PAN__


