/*
 ============================================================================
 Name		: Glaze_Core.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include <eikstart.h>
#include "Glaze_CoreApplication.h"

LOCAL_C CApaApplication* NewApplication() {
	return new CGlaze_CoreApplication;
}

GLDEF_C TInt E32Main() {
	return EikStart::RunApplication(NewApplication);
}

