/*
 * Core.cpp
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#include "Core.h"

// System includes
#include <e32base.h>

// ================= MEMBER FUNCTIONS =======================
CGlazeCore* CGlazeCore::NewL() {
	CGlazeCore* self = new (ELeave) CGlazeCore();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

CGlazeCore::CGlazeCore() {
	iReady = EFalse;

	iXml = XMLBuffer::NewL();
	iXml->Parse(_L("core.xml"));

	iLibrary = CLocalLibrary::NewL(iXml->getNumLibs());
	iLibrary->addFrontEndLib(iXml->getFName());
	TInt index = 1;
	for (TInt i = 0; i < iXml->getNumVLibs(); i++) {
		iLibrary->addVisualizationLib(iXml->getVName(i));
		index++;
		for (TInt j = 0; j < iXml->getNumILibs(i); j++) {
			iLibrary->addInputLib(iXml->getIName(i, j));
			index++;
		}
	}
	delete iXml;
	TInt Sx = iLibrary->FE_getResolution(0);
	TInt Sy = iLibrary->FE_getResolution(1);
	iLibrary->V_execute(Sx, Sy);

	iPeriodic = CPeriodic::NewL(CActive::EPriorityIdle);
	iPeriodic->Start(100, 100, TCallBack(CGlazeCore::DrawCallBack, this));

}

CGlazeCore::~CGlazeCore() {
	delete iPeriodic;
	delete iLibrary;
}

void CGlazeCore::ConstructL(void) {
}

TInt CGlazeCore::DrawCallBack(TAny* aInstance) {
	CGlazeCore* instance = (CGlazeCore*) aInstance;
	instance->iFrame++;
	instance->renderLoop(instance->iFrame);
	if (!(instance->iFrame % 100))
		User::ResetInactivityTime();
	if (!(instance->iFrame % 50))
		User::After(0);
	return 0;
}

void CGlazeCore::renderLoop(TInt aFrame) {
	iLibrary->FE_clearDevice();
	if (!iReady)
		iReady = iLibrary->V_ready();
	else
		iLibrary->V_render(aFrame);
	iLibrary->FE_swapBuffers();
}

void CGlazeCore::setScreenSize(TInt w, TInt h) {
	iLibrary->V_setScreenSize(w, h);
}

void CGlazeCore::handleKey(TUint key, TBool pressed) {
	iLibrary->V_handleInput(key, pressed);
}
