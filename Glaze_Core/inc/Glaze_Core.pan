/*
 ============================================================================
 Name		: Glaze_Core.pan
 Author	  : 
 Copyright   : Your copyright notice
 Description : This file contains panic codes.
 ============================================================================
 */

#ifndef __GLAZE_CORE_PAN__
#define __GLAZE_CORE_PAN__

/** Glaze_Core application panic codes */
enum TGlaze_CorePanics
	{
	EGlaze_CoreUi = 1
	// add further panics here
	};

inline void Panic(TGlaze_CorePanics aReason)
	{
	_LIT(applicationName, "Glaze_Core");
	User::Panic(applicationName, aReason);
	}

#endif // __GLAZE_CORE_PAN__
