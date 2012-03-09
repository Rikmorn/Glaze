/*
============================================================================
 Name        : Target.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Target.h - header file
============================================================================
*/

#ifndef TARGET_H
#define TARGET_H

#include "IInput.h"
#include "IVisualization.h"
#include "Utils3d.h" // Utilities (texmanager, textures etc.)
#include "Glutils.h"
#include "GlazeTextures.h"
#include "GlazeFont.h"

#include "GlazeMesh.h"
#include "GlazeAnimation.h"

#include "Nightvision.pan"      	// panic codes
#include <e32std.h>
#include <GLES/egl.h>

#define PIOVER80 0.0174532925

class CNightvision : public IVisualization {
	/* interface methods */
public:
	IMPORT_C static IVisualization* NewL();
	CNightvision();

	virtual void registerInput(IInput *);
	virtual void render(TInt);
	virtual void execute(TUint, TUint);
	virtual void cleanup();
	virtual void setScreenSize(TInt, TInt);
	virtual void handleInput(TUint, TBool);
	virtual TBool ready();	

private:
	void initVisualization(TUint w, TUint h);
	void renderLoop(TInt iFrame);
	void assignTextures();
	void updateUserInput();
	void queryInputLib(TInt, TAny *, const TDesC&);
	void animate();
	void renderBackground();

private:
	TUint iWidth;
	TUint iHeight;
	TUint iKey; // key press code
	TInt iFrame;
	TReal iPanX, iPanY;
	TReal iHeading;
	TBool iKeyPressed;
	
	TReal posx, posz;
	
	CGlazeAnimationManager* iAnimManager;
	CGlazeMesh* iCube; 
		
	RPointerArray<IInput>* iInputLib; // keep all needed input DLL
	RArray<TReal64>* iGPSData;
	RArray<TReal>* iShakeData;
	RPointerArray<CFbsBitmap>* iCameraFrame;
	CGlazeTextures *iTextures;
	CGlazeFont *iFont;
	
	GLuint iViewFinderTex;
	TInt* positionBuffer;
	TBool round;
};

#endif // TAGCLOUD_H

// End of File
