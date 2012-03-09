/*
 ============================================================================
 Name		: Glaze_CoreAppUi.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares UI class for application.
 ============================================================================
 */

#ifndef __GLAZE_COREAPPUI_h__
#define __GLAZE_COREAPPUI_h__

// INCLUDES
#include <aknappui.h>
#include <coeaui.h>

// FORWARD DECLARATIONS
class CGlaze_CoreAppView;

// CLASS DECLARATION
/**
 * CGlaze_CoreAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */

class CGlaze_CoreAppUi : public CAknAppUi 
	{
public:
	// Constructors and destructor

	/**
	 * ConstructL.
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CGlaze_CoreAppUi.
	 * C++ default constructor. This needs to be public due to
	 * the way the framework constructs the AppUi
	 */
	CGlaze_CoreAppUi();

	/**
	 * ~CGlaze_CoreAppUi.
	 * Virtual Destructor.
	 */
	virtual ~CGlaze_CoreAppUi();

private:
	// Functions from base classes

	/**
	 * From CEikAppUi, HandleCommandL.
	 * Takes care of command handling.
	 * @param aCommand Command to be handled.
	 */
	void HandleCommandL(TInt aCommand);

	/**
	 *  HandleStatusPaneSizeChange.
	 *  Called by the framework when the application status pane
	 *  size is changed.
	 */
	void HandleStatusPaneSizeChange();
	
	virtual TKeyResponse HandleKeyEventL(
	            const TKeyEvent& aKeyEvent,TEventCode aType);

private:
	// Data

	/**
	 * The application view
	 * Owned by CGlaze_CoreAppUi
	 */
	CGlaze_CoreAppView* iAppView;

	};

#endif // __GLAZE_COREAPPUI_h__
// End of File
