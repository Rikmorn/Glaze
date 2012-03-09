/*
 * LibraryManager.cpp
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#include "LibraryManager.h"

void CLibraryManager::FE_cleanup() {
	iFrontEnd->cleanup();
}

void CLibraryManager::FE_clearDevice() {
	iFrontEnd->clearDevice();
}

void CLibraryManager::FE_swapBuffers() {
	iFrontEnd->swapBuffers();
}

TInt CLibraryManager::FE_getResolution(TInt aCoord) {
	return (aCoord) ? iFrontEnd->getScreenResolutionY()
			: iFrontEnd->getScreenResolutionX();
}

void CLibraryManager::V_render(TUint aFrame) {
	for (TInt i = 0; i < iVisualizations->Count(); i++)
		(*iVisualizations)[i]->render(aFrame);
}

TBool CLibraryManager::V_ready() {
	for (TInt i = 0; i < iVisualizations->Count(); i++)
		if (!(*iVisualizations)[i]->ready())
			return EFalse;
	return ETrue;
}

void CLibraryManager::V_execute(TUint w, TUint h) {
	for (TInt i = 0; i < iVisualizations->Count(); i++)
		(*iVisualizations)[i]->execute(w, h);
}

void CLibraryManager::V_setScreenSize(TUint w, TUint h) {
	for (TInt i = 0; i < iVisualizations->Count(); i++)
		(*iVisualizations)[i]->setScreenSize(w, h);
}

void CLibraryManager::V_handleInput(TUint key, TBool pressed) {
	for (TInt i = 0; i < iVisualizations->Count(); i++)
		(*iVisualizations)[i]->handleInput(key, pressed);
}
