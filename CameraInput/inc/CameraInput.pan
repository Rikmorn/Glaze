/*
============================================================================
 Name        : InputDll.pan
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Panic codes
============================================================================
*/

#ifndef __INPUT_PAN__
#define __INPUT_PAN__

/** Input panic codes */
enum TInputPanic
	{
	EInputDllNullPointer
	};

inline void Panic(TInputPanic aReason)
	{
	_LIT(applicationName,"Input");
	User::Panic(applicationName, aReason);
	}

#endif  // __INPUTDLL_PAN__


