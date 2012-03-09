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
}

CGlazeMesh::~CGlazeMesh() {
}

void CGlazeMesh::ConstructL(void) {
}

void CGlazeMesh::renderMesh(CGlazeTextures* aTex) {
	
	glTranslatef(iPosx, iPosy, iPosz);
	/*
	//HOME TESTING
	glBindTexture(GL_TEXTURE_2D, aTex->getTexture(1).iID);
	glTexCoordPointer(2, GL_BYTE, 0, streetTextTemp);
	glVertexPointer(3, GL_FLOAT, 0, streetVertTemp);
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE,
			streetFaceTemp);
	*/
	// ground
	glBindTexture(GL_TEXTURE_2D, aTex->getTexture(1).iID); // ground tex
	glTexCoordPointer(2, GL_BYTE, 0, groundTex);
	glVertexPointer(3, GL_FLOAT, 0, groundVert);
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE,
	streetFace);
	
	// cantina
	glBindTexture(GL_TEXTURE_2D, aTex->getTexture(5).iID); // cantina tex
	glTexCoordPointer(2, GL_BYTE, 0, cantinaTexdata);
	glVertexPointer(3, GL_FLOAT, 0, cantinaVertData);
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_BYTE,
	objCubeFaceData);

	// tecnopolo
	glBindTexture(GL_TEXTURE_2D, aTex->getTexture(6).iID); // tecn tex
	glTexCoordPointer(2, GL_BYTE, 0, tecnTexdata);
	glVertexPointer(3, GL_FLOAT, 0, tecnVertData);
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_BYTE,
	objCubeFaceData);

	// adj
	glBindTexture(GL_TEXTURE_2D, aTex->getTexture(2).iID); // adj tex
	glTexCoordPointer(2, GL_BYTE, 0, adjTexdata);
	glVertexPointer(3, GL_FLOAT, 0, adjVertData);
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_BYTE,
	objCubeFaceData);

	// adj1
	glBindTexture(GL_TEXTURE_2D, aTex->getTexture(3).iID); // adj1 tex
	glTexCoordPointer(2, GL_BYTE, 0, adj1Texdata);
	glVertexPointer(3, GL_FLOAT, 0, adj1VertData);
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_BYTE,
	objCubeFaceData);

	// adj2
	glBindTexture(GL_TEXTURE_2D, aTex->getTexture(4).iID); // adj2 tex
	glTexCoordPointer(2, GL_BYTE, 0, adj2Texdata);
	glVertexPointer(3, GL_FLOAT, 0, adj2VertData);
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_BYTE,
	objCubeFaceData);
	
	// campus
	glBindTexture(GL_TEXTURE_2D, aTex->getTexture(7).iID); // campus tex
	glTexCoordPointer(2, GL_BYTE, 0, campusTexdata);
	glVertexPointer(3, GL_FLOAT, 0, campusVertData);
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_BYTE,
	objCubeFaceData);
}

void CGlazeMesh::updateAnimation(TInt aAnim, TInt64 aDTime) {
	switch (aAnim) {
	case 0:
		animation1(aDTime);
		break;
	}
}

TInt CGlazeMesh::getNumAminations() {
	return iAnimNum;
}

void CGlazeMesh::updateVelocity(TReal aVelx, TReal aVely, TReal aVelz) {
	iVelx = aVelx;
	iVely = aVely;
	iVelz = aVelz;
}

void CGlazeMesh::updateInitialPosition(TReal iPx, TReal iPy, TReal iPz) {
	iPosix = iPx;
	iPosiy = iPy;
	iPosiz = iPz;
}

void CGlazeMesh::animation1(TInt64 aDTime /*miliseconds*/) {
	// calculate position at aDTime
	iPosx = iPosix + iVelx * aDTime;
	iPosy = iPosiy + iVely * aDTime;
	iPosz = iPosiz + iVelz * aDTime;
}
