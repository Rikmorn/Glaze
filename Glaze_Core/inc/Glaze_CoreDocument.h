/*
 ============================================================================
 Name		: Glaze_CoreDocument.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares document class for application.
 ============================================================================
 */

#ifndef __GLAZE_COREDOCUMENT_h__
#define __GLAZE_COREDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CGlaze_CoreAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CGlaze_CoreDocument application class.
 * An instance of class CGlaze_CoreDocument is the Document part of the
 * AVKON application framework for the Glaze_Core example application.
 */
class CGlaze_CoreDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CGlaze_CoreDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CGlaze_CoreDocument.
	 */
	static CGlaze_CoreDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CGlaze_CoreDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CGlaze_CoreDocument.
	 */
	static CGlaze_CoreDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~CGlaze_CoreDocument
	 * Virtual Destructor.
	 */
	virtual ~CGlaze_CoreDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CGlaze_CoreAppUi object and return a pointer to it.
	 * The object returned is owned by the Uikon framework.
	 * @return Pointer to created instance of AppUi.
	 */
	CEikAppUi* CreateAppUiL();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CGlaze_CoreDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CGlaze_CoreDocument(CEikApplication& aApp);

	};

#endif // __GLAZE_COREDOCUMENT_h__
// End of File
