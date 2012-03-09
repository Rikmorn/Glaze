/*
 ============================================================================
 Name		: Glaze_CoreAppUi.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : CGlaze_CoreAppUi implementation
 ============================================================================
 */
// INCLUDE FILES
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <hlplch.h>
#include <Glaze_Core_0xE3243CFD.rsg>
#include "Glaze_Core_0xE3243CFD.hlp.hrh"
#include "Glaze_Core.hrh"
#include "Glaze_Core.pan"
#include "Glaze_CoreApplication.h"
#include "Glaze_CoreAppUi.h"
#include "Glaze_CoreAppView.h"
_LIT( KFileName, "C:\\private\\E3243CFD\\Glaze_Core.txt" );
_LIT( KText, "Hello World!");

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CGlaze_CoreAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CGlaze_CoreAppUi::ConstructL() {
	// Initialise app UI with standard value.
	//BaseConstructL(CAknAppUi::EAknEnableSkin);
	BaseConstructL();

	// Create view object
	iAppView = CGlaze_CoreAppView::NewL(ClientRect());

}
// -----------------------------------------------------------------------------
// CGlaze_CoreAppUi::CGlaze_CoreAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CGlaze_CoreAppUi::CGlaze_CoreAppUi() {
	// No implementation required
}

// -----------------------------------------------------------------------------
// CGlaze_CoreAppUi::~CGlaze_CoreAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CGlaze_CoreAppUi::~CGlaze_CoreAppUi() {
	if (iAppView) {
		delete iAppView;
		iAppView = NULL;
	}

}

TKeyResponse CGlaze_CoreAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent,
		TEventCode aType) {
	TUint scan = aKeyEvent.iScanCode;
	if (aType == EEventKeyDown)
		iAppView->HandleInput(scan, ETrue);
	if (aType == EEventKeyUp)
		iAppView->HandleInput(scan, EFalse);
}

// -----------------------------------------------------------------------------
// CGlaze_CoreAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CGlaze_CoreAppUi::HandleCommandL(TInt aCommand) {
	switch (aCommand) {
	case EEikCmdExit:
	case EAknSoftkeyExit:
		Exit();
		break;
		
	case ECommand1: {
	}
		break;
	case ECommand2: {
	}
		break;
	case EHelp: {
	}
		break;
	case EAbout: {
	}
		break;
	default:
		Panic(EGlaze_CoreUi);
		break;
	}
}
// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CGlaze_CoreAppUi::HandleStatusPaneSizeChange() {
	iAppView->SetRect(ClientRect());
}

// End of File
