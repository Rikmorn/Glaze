/*
 ============================================================================
 Name		: Glaze_CoreDocument.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : CGlaze_CoreDocument implementation
 ============================================================================
 */

// INCLUDE FILES
#include "Glaze_CoreAppUi.h"
#include "Glaze_CoreDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CGlaze_CoreDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGlaze_CoreDocument* CGlaze_CoreDocument::NewL(CEikApplication& aApp) {
	CGlaze_CoreDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
}

// -----------------------------------------------------------------------------
// CGlaze_CoreDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGlaze_CoreDocument* CGlaze_CoreDocument::NewLC(CEikApplication& aApp) {
	CGlaze_CoreDocument* self = new (ELeave) CGlaze_CoreDocument(aApp);

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

// -----------------------------------------------------------------------------
// CGlaze_CoreDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CGlaze_CoreDocument::ConstructL() {
	// No implementation required
}

// -----------------------------------------------------------------------------
// CGlaze_CoreDocument::CGlaze_CoreDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CGlaze_CoreDocument::CGlaze_CoreDocument(CEikApplication& aApp) :
	CAknDocument(aApp) {
	// No implementation required
}

// ---------------------------------------------------------------------------
// CGlaze_CoreDocument::~CGlaze_CoreDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CGlaze_CoreDocument::~CGlaze_CoreDocument() {
	// No implementation required
}

// ---------------------------------------------------------------------------
// CGlaze_CoreDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CGlaze_CoreDocument::CreateAppUiL() {
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new (ELeave) CGlaze_CoreAppUi;
}

// End of File
