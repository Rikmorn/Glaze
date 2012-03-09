/*
 * GlazeMesh.cpp
 *
 *  Created on: 2/Mai/2009
 *      Author: Daemoniorum
 */

#include "GlazeMesh.h"

CGlazeMesh* CGlazeMesh::NewL() {
	CGlazeMesh* self = new (ELeave) CGlazeMesh();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

CGlazeMesh::CGlazeMesh() {
	iAnimNum = 1;
	iPosx = 0;
	iPosy = 0;
	iPosz = 0;
	iTabInfo.Append(_L("Obj - Tab A"));
	iTabInfo.Append(_L("Obj - Tab B"));
	iTabInfo.Append(_L("Obj - Tab C"));
	iTabInfo.Append(_L("Obj - Tab D"));
	iFont = CGlazeFont::NewL(8, 13, 240, 320, TEX_WIDTH, TEX_HEIGHT);
	iPosix = 0;
	iPosiy = 0;
	iPosiz = 0;
	iPosfx = 0;
	iPosfy = 0;
	iPosfz = 0;
}

CGlazeMesh::~CGlazeMesh() {
	iTabInfo.Close();
	delete iFont;
}

void CGlazeMesh::ConstructL(void) {
}

void CGlazeMesh::renderMesh() {
	glTranslatef(iPosx, iPosy, iPosz);
	glScalex(50 << 16, 50 << 16, 50 << 16);
	glBindTexture(GL_TEXTURE_2D, iIcon.iID); // target tex
	glTexCoordPointer(2, GL_BYTE, 0, camTex);
	glVertexPointer(3, GL_FLOAT, 0, targetVert);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE,
	camFace);
}

void CGlazeMesh::setupAnimation(TInt aAnim, TInt64 aTotalTime) {
	switch (aAnim) {
	case 0:
		iVelx = (TReal) (iPosfx - iPosix) / (TReal) aTotalTime;
		iVely = (TReal) (iPosfy - iPosiy) / (TReal) aTotalTime;
		iVelz = (TReal) (iPosfz - iPosiz) / (TReal) aTotalTime;
		break;
	}
}

void CGlazeMesh::updateAnimation(TInt aAnim, TInt64 aDTime) {
	switch (aAnim) {
	case 0:
		animation1(aDTime);
		break;
	}
}

void CGlazeMesh::animationComplete(TInt aAnim) {
	switch (aAnim) {
	case 0:
		// change positions
		iPosix = iPosfx;
		iPosiy = iPosfy;
		iPosiz = iPosfz;
		break;
	}
}

TInt CGlazeMesh::getNumAminations() {
	return iAnimNum;
}

void CGlazeMesh::animation1(TInt64 aDTime /*miliseconds*/) {
	// calculate position at aDTime
	iPosx = iPosix + iVelx * aDTime;
	iPosy = iPosiy + iVely * aDTime;
	iPosz = iPosiz + iVelz * aDTime;
}

void CGlazeMesh::getCurrentPos(TReal &aX, TReal &aY, TReal &aZ) {
	aX = iPosx;
	aY = iPosy;
	aZ = iPosz;
}

void CGlazeMesh::setWorldPosition(TInt aX, TInt aY, TInt aZ) {
	iWorldX = aX;
	iWorldY = aY;
	iWorldZ = aZ;
}

void CGlazeMesh::addInfo(TUint aTabNum, const TDesC &aInfo) {
	TBuf<100> info(aInfo);
	iTabInfo[aTabNum] = info;
}

void CGlazeMesh::setTexture(TTexture aIcon) {
	iIcon = aIcon;
}

void CGlazeMesh::setFontTexture(TTexture aFont) {
	iFont->fontTex(aFont);
}

TBool CGlazeMesh::hasMoved(TInt aUX, TInt aUY, TInt aUZ) {
	// movement initial position = iPosix,y,z
	// final position dx,y,z
	TInt dx = iWorldX - aUX;
	TInt dy = iWorldY - aUY;
	TInt dz = iWorldZ - aUZ;
	if (dx != iPosfx || dy != iPosfy || dz != iPosfz) {
		iPosfx = dx;
		iPosfy = dy;
		iPosfz = dz;
		return ETrue;
	} else {
		return EFalse;
	}
}

/* 
 * in a real application at least this method would be abstract
 * this way each target could have any content the developer desired
 */
void CGlazeMesh::renderContent(TInt aActiveTab) {
	if (aActiveTab >= 4) {
		return;
	}
	iFont->setScale(1, 1, 1);
	iFont->drawText3D(iMeshName, 15, 250, 0);
	// 25 chars per line
	const TInt charNumber = 26;
	TInt size = iTabInfo[aActiveTab].Length();
	if (size < charNumber) {
		iFont->drawText3D(iTabInfo[aActiveTab], 15, 230, 0);
	} else {
		TBuf<charNumber> stringb;
		TInt lineN = 0;
		for (TInt i = 0; i < size; i++) {
			TInt j = i % charNumber;
			if (j == 0 && i > 1) {
				iFont->drawText3D(stringb, 15, 230 - (20 * lineN), 0);
				lineN++;
				stringb.Zero();
			}
			stringb.Append(iTabInfo[aActiveTab][i]);
		}
		iFont->drawText3D(stringb, 15, 230 - (20 * lineN), 0);
	}
	
}

void CGlazeMesh::addName(const TDesC& aName) {
	iMeshName.Copy(aName);
}

TPtrC CGlazeMesh::getName() {
	return iMeshName;
}
