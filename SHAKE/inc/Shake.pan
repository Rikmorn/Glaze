/*
============================================================================
 Name        : InputDll.pan
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Panic codes
============================================================================
*/

#ifndef __SHAKEDLL_H__
#define __SHAKEDLL_H__

/** Input panic codes */
enum TShakePanic
	{
	EShakeDllNullPointer
	};

inline void Panic(TShakePanic aReason)
	{
	_LIT(applicationName,"Shake");
	User::Panic(applicationName, aReason);
	}

#endif  // __INPUTDLL_PAN__


