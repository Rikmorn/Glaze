/*
 * CArrow.cpp
 *
 *  Created on: 20/Mai/2009
 *      Author: Daemoniorum
 */

#include "CArrow.h"

CGlazeArrow* CGlazeArrow::NewL() {
	CGlazeArrow* self = new (ELeave) CGlazeArrow();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

CGlazeArrow::CGlazeArrow() {

}

CGlazeArrow::~CGlazeArrow() {
	iXcoords.Close();
	iYcoords.Close();
	iZcoords.Close();
}

void CGlazeArrow::ConstructL(void) {
}

void CGlazeArrow::renderArrows(CGlazeTextures* aTex, TReal rot) {
	TInt px, py, pz;
	px = 0;
	py = -100;
	pz = 0;
	GLfloat aBody[6];
	/*iXcoords[0] = -900;
	iYcoords[0] = 0;
	iZcoords[0] = -460;*/
	for (TInt i = 0; i < iXcoords.Count(); i++) {
		glLoadIdentity();
		//glTranslatex(px << 16, py << 16, pz << 16);
		glTranslatex(px << 16, py << 16, -450 << 16);
		glRotatef( rot, 0, 1, 0 ); // equal rotatio to scene should do it
		//glTranslatex(0 << 16, 0 << 16, -450 << 16); // manual camera translating
		TReal length;
		TReal a2;
		TReal b2;
		TReal c2;
		Math::Pow(a2, iXcoords[i] - px, 2);
		Math::Pow(b2, iYcoords[i] - py, 2);
		Math::Pow(c2, iZcoords[i] - pz, 2);
		Math::Sqrt(length, a2 + b2 + c2);
		TReal t = iXcoords[i] - (TReal) px;
		TReal t1 = iYcoords[i] - (TReal) py;
		TReal t2 = iZcoords[i] - (TReal) pz;
		aBody[0] = 0;
		aBody[1] = 0;
		aBody[2] = 0;
		aBody[3] = aBody[0] + (t / length) * 20;
		aBody[4] = aBody[1] + (t1 / length) * 20; 
		aBody[5] = aBody[2] + (t2 / length) * 20;
		// Draw Line
		glDisable(GL_TEXTURE_2D);
		glLineWidth(1);
		glVertexPointer(3, GL_FLOAT, 0, aBody);
		glDrawElements(GL_LINES, 5, GL_UNSIGNED_BYTE, arrowBodyFace);
		glEnable(GL_TEXTURE_2D);
		
		GLfloat aHeadBody[12];
		aHeadBody[0] = aBody[3]-4;
		aHeadBody[1] = aBody[4]+4;
		aHeadBody[2] = aBody[5];
		
		aHeadBody[3] = aBody[3]+4;
		aHeadBody[4] = aBody[4]+4;
		aHeadBody[5] = aBody[5];
		
		aHeadBody[6] = aBody[3]+4;
		aHeadBody[7] = aBody[4]-4;
		aHeadBody[8] = aBody[5];
		
		aHeadBody[9] = aBody[3]-4;
		aHeadBody[10] = aBody[4]-4;
		aHeadBody[11] = aBody[5];
		
		// Draw arrow's head
		
		 glBindTexture(GL_TEXTURE_2D, aTex->getTexture(1).iID); // target tex
		 glTexCoordPointer(2, GL_BYTE, 0, arrowTex);
		 glVertexPointer(3, GL_FLOAT, 0, aHeadBody);
		 glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, arrowFace);
		
	}
}

void CGlazeArrow::createCoords(TInt aNum) {
	for (TInt i = 0; i < aNum; i++) {
		iXcoords.Append(0);
		iYcoords.Append(0);
		iZcoords.Append(0);
	}
}

void CGlazeArrow::updateObjCoords(TInt aIndex, TReal aX, TReal aY, TReal aZ) {
	iXcoords[aIndex] = aX;
	iYcoords[aIndex] = aY;
	iZcoords[aIndex] = aZ;
}

void CGlazeArrow::resetCoords() {
	iXcoords.Reset();
	iYcoords.Reset();
	iZcoords.Reset();
}
