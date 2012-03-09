/*
============================================================================
 Name        : OpenGLFEDll.pan
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Panic codes
============================================================================
*/

#ifndef __OPENGLFEDLL_PAN__
#define __OPENGLFEDLL_PAN__

/** OpenGLFEDll panic codes */
enum TOpenGLFEDllPanic
	{
	EOpenGLFEDllNullPointer
	};

inline void Panic(TOpenGLFEDllPanic aReason)
	{
	_LIT(applicationName,"OpenGLFE");
	User::Panic(applicationName, aReason);
	}

#endif  // __OPENGLFEDLL_PAN__


