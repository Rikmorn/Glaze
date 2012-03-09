/*
 * CArrow.h
 *
 *  Created on: 20/Mai/2009
 *      Author: Daemoniorum
 */

#ifndef CARROW_H_
#define CARROW_H_

#include "GlazeTextures.h"
#include <GLES/egl.h>

#define RADTODEG 57.29577951

class CGlazeArrow{
public:
	static CGlazeArrow* NewL();
	virtual ~CGlazeArrow();

protected:
	CGlazeArrow();
	void ConstructL(void);

public:
	void renderArrows(CGlazeTextures* aTex, TReal rot);
	void updateObjCoords(TInt aIndex, TReal aX,TReal aY,TReal aZ);
	void createCoords(TInt aNum);
	void resetCoords();
	
private:
	RArray<TReal> iXcoords;
	RArray<TReal> iYcoords;
	RArray<TReal> iZcoords;
};

static GLfloat arrowBody[] = { 
		0, 0, 
		0, 20
		};

static GLubyte arrowBodyFace[] = { 0,1,2,3,4,5 };

static GLfloat arrowVert[] = { 
		-4, 24, 0, 
		4, 24, 0,
		4, 16, 0, 
		-4, 16, 0
		};

static GLbyte arrowTex[] = { 
		-127, -127, 
		126, -127, 
		126, 126, 
		-127, 126 
		};

static GLubyte arrowFace[] = { 
		1, 0, 3, 
		1, 3, 2 
		};


#endif /* CARROW_H_ */
