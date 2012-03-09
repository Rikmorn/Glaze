 /*
 ============================================================================
 Name		: Glaze_CoreApplication.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "Glaze_Core.hrh"
#include "Glaze_CoreDocument.h"
#include "Glaze_CoreApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CGlaze_CoreApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CGlaze_CoreApplication::CreateDocumentL() {
	// Create an Glaze_Core document, and return a pointer to it
	return CGlaze_CoreDocument::NewL(*this);
}

// -----------------------------------------------------------------------------
// CGlaze_CoreApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CGlaze_CoreApplication::AppDllUid() const {
	// Return the UID for the Glaze_Core application
	return KUidGlaze_CoreApp;
}

// End of File
