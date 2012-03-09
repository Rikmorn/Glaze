/*
 * LocalLibrary.cpp
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#include "LocalLibrary.h"

CLocalLibrary* CLocalLibrary::NewL(TUint aNumLibs) {
	CLocalLibrary* self = new (ELeave) CLocalLibrary(aNumLibs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

CLocalLibrary::CLocalLibrary(TUint aNumLibs) {
	iNumLibs = aNumLibs;
	iLibs = new (ELeave) RLibrary[iNumLibs];
	iVisualizations = new (ELeave) RPointerArray<IVisualization> ;
	iCurrentLib = 0;
	iCurrentVis = -1;
}

CLocalLibrary::~CLocalLibrary() {
	for (TInt i = 0; i < iVisualizations->Count(); i++)
		(*iVisualizations)[i]->cleanup();
	iVisualizations->ResetAndDestroy();
	iVisualizations->Close();
	delete iVisualizations;
	iFrontEnd->cleanup();
	delete iFrontEnd;
	for (TUint i = 0; i < iNumLibs; i++)
		iLibs[i].Close();
	delete[] iLibs;
}

void CLocalLibrary::ConstructL() {
}

void CLocalLibrary::addFrontEndLib(const TDesC &aName) {
	iFrontEnd = (IFrontEnd*) OpenLib(aName)();

	iCurrentLib++;
}

void CLocalLibrary::addVisualizationLib(const TDesC &aName) {
	IVisualization* v = (IVisualization*) OpenLib(aName)();
	iVisualizations->AppendL(v);

	iCurrentLib++;
	iCurrentVis++;
}

void CLocalLibrary::addInputLib(const TDesC &aName) {
	IInput* in = (IInput*) OpenLib(aName)();
	(*iVisualizations)[iCurrentVis]->registerInput(in);

	iCurrentLib++;
}

TLibraryFunction CLocalLibrary::OpenLib(const TDesC &aName) {
	User::LeaveIfError(iLibs[iCurrentLib].Load(aName));
	return iLibs[iCurrentLib].Lookup(1);
}
