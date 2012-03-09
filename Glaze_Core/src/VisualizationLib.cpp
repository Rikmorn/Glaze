/*
 * VisualizationLib.cpp
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#include "VisualizationLib.h"
#include <e32base.h>
// ================= MEMBER FUNCTIONS =======================
CVisualizationLib* CVisualizationLib::NewL(const TDesC& aName) {
	CVisualizationLib* self = new (ELeave) CVisualizationLib(aName);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

CVisualizationLib::CVisualizationLib(const TDesC& aName) {
	iName = aName;
}

CVisualizationLib::~CVisualizationLib() {
	iInputName.Reset();
	iInputName.Close();
}

void CVisualizationLib::ConstructL(void) {
}

void CVisualizationLib::addInputLib(const TDesC& aName) {
	TBuf<100> name(aName);
	iInputName.AppendL(name);
}

void CVisualizationLib::numStates(TUint aNum) {
	iNum = aNum;
}

TPtrC CVisualizationLib::getVName() {
	return iName;
}

TPtrC CVisualizationLib::getIName(TUint aPos) {
	return iInputName[aPos];
}

TUint CVisualizationLib::getNumInputs() {
	return iInputName.Count();
}
