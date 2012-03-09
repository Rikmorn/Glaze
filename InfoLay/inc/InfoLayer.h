/*
 * InfoLayer.h
 *
 *  Created on: 22/Mai/2009
 *      Author: Daemoniorum
 */

#ifndef INFOLAYER_H_
#define INFOLAYER_H_

#include "GlazeAnimation.h"
#include "GlazeTextures.h"
#include "GlazeCommon.h"
#include "MeshList.h"

#include <GLES/egl.h>

class CInfoLayer: public MAnimationObserver {
public:
	static CInfoLayer* NewL();
	virtual ~CInfoLayer();

protected:
	CInfoLayer();
	void ConstructL(void);

public:
	void renderGUI(CGlazeTextures* aTex, CMeshList *iMeshList, TInt selectedMesh);
	TBool toggleGUI();

private:
	virtual void setupAnimation(TInt aAnim, TInt64 aTotalTime);
	virtual void updateAnimation(TInt aAnim, TInt64 aDTime);
	virtual void animationComplete(TInt aAnim);
	virtual TInt getNumAminations();

	void set2D();
	void unset2D();
	void left(TInt64 aDTime);
	void right(TInt64 aDTime);
	void appear(TInt64 aDTime);
	void disappear(TInt64 aDTime);
	
private:
	TBool iAcceptIn;
	TBool iActive;
	TUint iCurrTab;
	TUint iAnimNum; 
	TReal iTargetX, iTargetY;
	TReal iInitX;
	TReal iFramePosY;
	TReal iFrameInitY;
	TReal iFrameVel;
	TReal iTabVel;
};

static GLfloat GUIVert[] = { 
		-128, 128, 0, 
		128, 128, 0,
		128, -128, 0, 
		-128, -128, 0
		};

static GLfloat chVert[] = { 
		-16, 16, 0, 
		16, 16, 0,
		16, -16, 0, 
		-16, -16, 0
		};

static GLbyte GUITex[] = { 
		-128, -128, 
		127, -128, 
		127, 127, 
		-128, 127 
		};

static GLubyte GUIFace[] = { 
		1, 0, 3, 
		1, 3, 2 
		};

#endif /* INFOLAYER_H_ */
