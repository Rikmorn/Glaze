/*
 * GlazeMesh.h
 *
 *  Created on: 2/Mai/2009
 *      Author: Daemoniorum
 */

/*
 * animations go through the same function over and over
 * just some can have values changing in the middle of the animation
 * while other don't.
 */

#ifndef GLAZEMESH_H_
#define GLAZEMESH_H_

#include "GlazeAnimation.h"
#include "GlazeTextures.h"
#include "GlazeCommon.h"
#include "GlazeFont.h"

#include <GLES/egl.h>

#define INFOLENGTH 100

class CGlazeMesh: public MAnimationObserver {
public:
	static CGlazeMesh* NewL();
	virtual ~CGlazeMesh();

protected:
	CGlazeMesh();
	void ConstructL(void);

public:
	void renderMesh();
	void getCurrentPos(TReal &aX, TReal &aY, TReal &aZ);
	TPtrC getName();
	
	void addInfo(TUint aTabNum, const TDesC &aInfo);
	void addName(const TDesC& aName);
	void setTexture(TTexture aIcon);
	void setFontTexture(TTexture aFont);
	void setWorldPosition(TInt aX, TInt aY, TInt aZ);
	TBool hasMoved(TInt aUX,TInt aUY,TInt aUZ);
	void renderContent(TInt aActiveTab);

private:
	virtual void setupAnimation(TInt aAnim, TInt64 aTotalTime);
	virtual void updateAnimation(TInt aAnim, TInt64 aDTime);
	virtual void animationComplete(TInt aAnim);
	virtual TInt getNumAminations();

	void animation1(TInt64 aDTime);

private:
	TUint iAnimNum;
	TReal iVelx, iVely, iVelz;
	TReal iPosx, iPosy, iPosz;
	TInt iWorldX,iWorldY,iWorldZ;
	
	TInt iPosix, iPosiy, iPosiz;
	TInt iPosfx, iPosfy, iPosfz;
	
	TBuf<50> iMeshName;
	TTexture iIcon;
	RArray< TBuf<INFOLENGTH> > iTabInfo;
	CGlazeFont *iFont;
};

//RES = 256, 128

static GLfloat camVert[] = { 
		-128, 64, 0, 
		128, 64, 0,
		128, -64, 0, 
		-128, -64, 0
		};

static GLfloat targetVert[] = { 
		-1, 1, 0, 
		1, 1, 0,
		1, -1, 0, 
		-1, -1, 0
		};

static GLbyte camTex[] = { 
		-128, -128, 
		127, -128, 
		127, 127, 
		-128, 127 
		};

static GLubyte camFace[] = { 
		1, 0, 3, 
		1, 3, 2 
		};

#endif /* GLAZEMESH_H_ */
