/*
 ============================================================================
 Name		: Glaze_CoreApplication.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __GLAZE_COREAPPLICATION_H__
#define __GLAZE_COREAPPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "Glaze_Core.hrh"

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidGlaze_CoreApp =
	{
	_UID3
	};

// CLASS DECLARATION

/**
 * CGlaze_CoreApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CGlaze_CoreApplication is the application part of the
 * AVKON application framework for the Glaze_Core example application.
 */
class CGlaze_CoreApplication : public CAknApplication
	{
public:
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID (KUidGlaze_CoreApp).
	 */
	TUid AppDllUid() const;

protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates CGlaze_CoreDocument document object. The returned
	 * pointer in not owned by the CGlaze_CoreApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	};

#endif // __GLAZE_COREAPPLICATION_H__
// End of File
