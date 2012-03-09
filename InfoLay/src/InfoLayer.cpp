/*
 * InfoLayer.cpp
 *
 *  Created on: 22/Mai/2009
 *      Author: Daemoniorum
 */

#include "InfoLayer.h"

CInfoLayer* CInfoLayer::NewL() {
	CInfoLayer* self = new (ELeave) CInfoLayer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

CInfoLayer::CInfoLayer() {
	iAcceptIn = ETrue;
	iActive = EFalse;
	iAnimNum = 4;
	iCurrTab = 0;
	iTargetX = -96;
	iTargetY = 304;
	iFramePosY = 390;
	iFrameVel = 0.0;
	iTabVel = 0.0;
}

CInfoLayer::~CInfoLayer() {

}

void CInfoLayer::ConstructL(void) {
}

void CInfoLayer::renderGUI(CGlazeTextures* aTex, CMeshList *iMeshList,
		TInt selectedMesh) {
	if (iActive) {
		set2D();
		glTranslatef(126, iFramePosY, 0);
		glBindTexture(GL_TEXTURE_2D, aTex->getTexture(4).iID); // target tex
		glTexCoordPointer(2, GL_BYTE, 0, GUITex);
		glVertexPointer(3, GL_FLOAT, 0, GUIVert);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE,
		GUIFace);

		// render pointer
		glTranslatef(iTargetX, 105, 0);
		glBindTexture(GL_TEXTURE_2D, aTex->getTexture(3).iID); // target tex
		glTexCoordPointer(2, GL_BYTE, 0, GUITex);
		glVertexPointer(3, GL_FLOAT, 0, chVert);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE,
		GUIFace);

		// write tab content
		//if (selectedMesh >= 0 && selectedMesh < iMeshList->meshNum()) {
			iMeshList->MeshContent(selectedMesh, iCurrTab);
		//}
		unset2D();
	}
}

TBool CInfoLayer::toggleGUI() {
	return !iActive;
}

void CInfoLayer::setupAnimation(TInt aAnim, TInt64 aTotalTime) {
	switch (aAnim) {
	case 0: // left
		if (iCurrTab == 0){
			iTabVel = 1.8;
		} else {
			iTabVel = -0.6;
		}
		iInitX = iTargetX;
		break;
	case 1: // right
		if (iCurrTab == 3){
			iTabVel = -1.8;
		} else {
			iTabVel = 0.6;
		}
		iInitX = iTargetX;
		break;
	case 2:
		iFrameInitY = iFramePosY;
		iFrameVel = 200.0 / (TReal) aTotalTime;
		iActive = ETrue;
		break;
	case 3:
		iFrameInitY = iFramePosY;
		iFrameVel = 200.0 / (TReal) aTotalTime;
		break;
	}
}
// careful with fighing animations
void CInfoLayer::updateAnimation(TInt aAnim, TInt64 aDTime) {
	switch (aAnim) {
	case 0:
		left(aDTime);
		break;
	case 1:
		right(aDTime);
		break;
	case 2:
		appear(aDTime);
		break;
	case 3:
		disappear(aDTime);
		break;
	}
}

void CInfoLayer::animationComplete(TInt aAnim) {
	switch (aAnim) {
	case 0:
		if (iCurrTab == 0){
			iCurrTab = 3;
		} else {
			iCurrTab--;
		}
		break;
	case 1:
		if (iCurrTab == 3){
			iCurrTab = 0;
		} else {
			iCurrTab++;
		}
		break;
	case 2:
		break;
	case 3:
		iActive = EFalse;
		break;
	}
}

TInt CInfoLayer::getNumAminations() {
	return iAnimNum;
}

void CInfoLayer::left(TInt64 aDTime /*miliseconds*/) {
	if (iActive) {
		iTargetX = iInitX + iTabVel * aDTime;
	}
}

void CInfoLayer::right(TInt64 aDTime /*miliseconds*/) {
	if (iActive) {
		iTargetX = iInitX + iTabVel * aDTime;
	}
}

void CInfoLayer::appear(TInt64 aDTime) {
	iFramePosY = iFrameInitY - iFrameVel * aDTime;
}

void CInfoLayer::disappear(TInt64 aDTime) {
	iFramePosY = iFrameInitY + iFrameVel * aDTime;
}

void CInfoLayer::set2D() {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrthox(0, 240 << 16, 0, 320 << 16, -1 << 16, 1 << 16);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void CInfoLayer::unset2D() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}
