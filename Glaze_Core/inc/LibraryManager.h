/*
 * LibraryManager.h
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#ifndef LIBRARYMANAGER_H_
#define LIBRARYMANAGER_H_

#include "IFrontEnd.h"
#include "IVisualization.h"
#include <e32base.h>    // CBase

class CLibraryManager {
protected:
	RLibrary* iLibs;
	TUint iNumLibs;
	RPointerArray<IVisualization>* iVisualizations;
	IFrontEnd* iFrontEnd;

public:
	virtual void addFrontEndLib(const TDesC &) = 0;
	virtual void addVisualizationLib(const TDesC &) = 0;
	virtual void addInputLib(const TDesC &) = 0;
	virtual void FE_cleanup();
	virtual void FE_swapBuffers();
	virtual void FE_clearDevice();
	virtual TInt FE_getResolution(TInt);
	virtual void V_render(TUint);
	virtual TBool V_ready();
	virtual void V_execute(TUint, TUint);
	virtual void V_setScreenSize(TUint, TUint);
	virtual void V_handleInput(TUint, TBool);
};

#endif /* LIBRARYMANAGER_H_ */
