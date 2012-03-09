/*
 ============================================================================
 Name        : TargetVisualization.cpp
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : TargetVisualization.cpp - source file
 ============================================================================
 */

#include "NightVision.h"

EXPORT_C IVisualization* CNightvision::NewL() {
	return new CNightvision();
}

CNightvision::CNightvision() {
	iInputLib = new (ELeave) RPointerArray<IInput> ;
	iGPSData = new (ELeave) RArray<TReal64> ;
	iShakeData = new (ELeave) RArray<TReal> ;
	iCameraFrame = new (ELeave) RPointerArray<CFbsBitmap> ;
	iKey = 0;
	iFrame = 0;
	iPanX = 0.0;
	iPanY = 0.0;
	posx = posz = 0.0;
	iHeading = 1.0;
	iKeyPressed = EFalse;
	positionBuffer = new TInt[6];
	for (TInt i = 0; i < 6; i++) {
		positionBuffer[i] = 0;
	}
	round = ETrue;
}

void CNightvision::execute(TUint iW, TUint iH) {
	TFileName iFile = _L("c:\\data\\others\\glaze\\textures\\");
	iTextures = CGlazeTextures::NewL(iFile, iW, iH);
	iFont = CGlazeFont::NewL(8, 13, iW, iH, TEX_WIDTH, TEX_HEIGHT);
	iAnimManager = CGlazeAnimationManager::NewL();
	iCube = CGlazeMesh::NewL();
	initVisualization(iW, iH);
}

/* cleanup after yourself */
void CNightvision::cleanup() {
	for (TInt i = 0; i < iInputLib->Count(); i++)
		(*iInputLib)[i]->cleanup();
	iInputLib->ResetAndDestroy();
	iInputLib->Close();
	delete iInputLib;
	iCameraFrame->ResetAndDestroy();
	iCameraFrame->Close();
	delete iCameraFrame;
	iGPSData->Close();
	delete iGPSData;
	iShakeData->Close();
	delete iShakeData;
	delete iFont;
	delete iTextures;
	delete iAnimManager;
	delete iCube;
	delete positionBuffer;
}

/* change screen size, this is called in during OpenGL initialization
 or if the screen needs to change size for some reason */
void CNightvision::setScreenSize(TInt iWidth, TInt iHeight) {
	glViewport(0, 0, iWidth, iHeight);
	// Recalculate the view frustrum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat aspectRatio = (GLfloat) (iWidth) / (GLfloat) (iHeight);
	glFrustumf(FRUSTUM_LEFT * aspectRatio, FRUSTUM_RIGHT * aspectRatio,
			FRUSTUM_BOTTOM, FRUSTUM_TOP,
			FRUSTUM_NEAR, FRUSTUM_FAR );
	glMatrixMode(GL_MODELVIEW);
}

/* initialize everything related to OpenGL and do everything that needs to
 be done before the render cycle */
void CNightvision::initVisualization(TUint w, TUint h) {
	iWidth = w;
	iHeight = h;

	iAnimManager->addMesh(iCube);

	iTextures->addTexture(_L("font.png"));
	iTextures->addTexture(_L("ground.png"));
	iTextures->addTexture(_L("adj1.png"));
	iTextures->addTexture(_L("adj2.png"));
	iTextures->addTexture(_L("adj3.png"));
	iTextures->addTexture(_L("cantina.png"));
	iTextures->addTexture(_L("tecn.png"));
	iTextures->addTexture(_L("campus.png"));
	iTextures->loadTextures();

	setScreenSize(iWidth, iHeight);

	glClearColor(0.f, 0.f, 1.f, 1.f);

	// Enables Depth Testing, texturing, backface culling and normalization
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	// Initialize appropriate texture matrix.
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1.0f / 255.0f, 1.0f / 255.0f, 1.0f);
	glTranslatef(128.0f, 128.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);

	// Depth and blend functions
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable vertex, normal, texture and color arrays.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Set the initial shading mode
	glShadeModel(GL_SMOOTH);

	// Use perspective correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void CNightvision::assignTextures() {
	iFont->fontTex(iTextures->getTexture(0));
}

void CNightvision::updateUserInput() {
	TReal temp;
	TReal temp1;
	TReal h = iHeading * PIOVER80;
	if (iKeyPressed) {
		switch (iKey) {
		
		 case 14: // left arrow
		 iHeading += 1.0;
		 iPanX = iHeading;
		 break;
		 case 15: // right arrow
		 iHeading -= 1.0;
		 iPanX = iHeading; // ipanx = roty
		 break;
		 /*
		case 14:
			Math::Sin(temp, h);
			Math::Cos(temp1, h);
			posx -= temp;
			posz -= temp1;
			break;
		case 15:
			Math::Sin(temp, h);
			Math::Cos(temp1, h);
			posx += temp;
			posz += temp1;
			break;
			*/
		case 16: //up arrow
			iPanY--; //lookupdown
			break;
		case 17: // down arrow
			iPanY++; //lookupdown
			break;
			
			 case 50:
			 Math::Sin(temp, h);
			 Math::Cos(temp1, h);
			 posx += temp;
			 posz += temp1;
			 break;
			 default:
			 Math::Sin(temp, h);
			 Math::Cos(temp1, h);
			 posx -= temp;
			 posz -= temp1;
			 break;
			 
		}
	}
}

TBool CNightvision::ready() {
	if (iTextures->GetState() == CGlazeTextures::EDone) {
		assignTextures();
		return ETrue;
	}
	return EFalse;
}

/* Target visualization's render cycle */
void CNightvision::renderLoop(TInt aFrame) {
	updateUserInput();
	queryInputLib(0, iGPSData, _L(""));
	queryInputLib(1, iShakeData, _L(""));

	/* 
	 * Convention is Latitude (N/S) y axis, Longitude (E/W) x axis
	 * geoid altitude z axis
	 */
	TInt userx = (TInt) ((*iGPSData)[0] * 1000000);
	TInt userz = (TInt) ((*iGPSData)[1] * 1000000);
	TInt usery = (TInt) (*iGPSData)[2];
	
	iHeading = (*iShakeData)[0];

	// UMA - 32.658020, -16.924250
	// HOME TEST - 32.647400, -16.844000

	TInt objx = 32658020;
	TInt objy = 39;
	TInt objz = -16924250;
	
	// calculate object position relative to user and change position of object
	TInt objcx = (TInt) (objx - userx);
	TInt objcy = (TInt) (objy - usery);
	TInt objcz = (TInt) (objz - userz);
	
	if (positionBuffer[3] != objcx || positionBuffer[4] != objcy
			|| positionBuffer[5] != objcz) {
		TInt time = 990;
		positionBuffer[0] = positionBuffer[3];
		positionBuffer[1] = positionBuffer[4];
		positionBuffer[2] = positionBuffer[5];
		positionBuffer[3] = objcx;
		positionBuffer[4] = objcy;
		positionBuffer[5] = objcz;

		TReal vx = (TReal) (positionBuffer[3] - positionBuffer[0])
				/ (TReal) time;
		TReal vy = (TReal) (positionBuffer[4] - positionBuffer[1])
				/ (TReal) time;
		TReal vz = (TReal) (positionBuffer[5] - positionBuffer[2])
				/ (TReal) time;
		iCube->updateInitialPosition(positionBuffer[0], positionBuffer[1],
				positionBuffer[2]);
		iCube->updateVelocity(vx, vy, vz);
		iAnimManager->playAnimation(0, 0, 800);
	}
	// First person camera orientation and position
	
	iPanX = iHeading;
	
	// render viewfinder texture 
	//glLoadIdentity();
	//renderBackground();
	
	//TReal scenerot = 360.0 - iPanX;
	glLoadIdentity();
	glRotatef(iPanY, 1, 0, 0);
	glRotatef(iPanX + 90, 0, 1, 0);
	//glRotatef(scenerot, 0, 1, 0);
	glTranslatef(posx*5, 0, posz*5); // manual camera translating
	
	// update animations
	iAnimManager->updateAnimations();

	// render object
	iCube->renderMesh(iTextures);
	/*
	// print orientation coordenates
	glLoadIdentity();
	iFont->setScale(1, 1, 1);
	TBuf<100> ori;
	TBuf<100> o;
	o.Num(iHeading, 64);
	ori.Append(_L("Ori: "));
	ori.Append(o);
	iFont->drawText3D(ori, -90, 0, -400);
	
	// print x coordenates
	glLoadIdentity();
	iFont->setScale(1, 1, 1);
	TBuf<100> lat;
	TBuf<100> l;
	l.Num(objcx, 64);
	lat.Append(_L("X: "));
	lat.Append(l);
	iFont->drawText3D(lat, -90, -20, -400);

	// print y coordenates
	glLoadIdentity();
	iFont->setScale(1, 1, 1);
	TBuf<100> longi;
	TBuf<100> lg;
	lg.Num(objcy, 64);
	longi.Append(_L("Y: "));
	longi.Append(lg);
	iFont->drawText3D(longi, -90, -40, -400);

	// print z coordenates
	glLoadIdentity();
	iFont->setScale(1, 1, 1);
	TBuf<100> alt;
	TBuf<100> a;
	a.Num(objcz, 64);
	alt.Append(_L("Z: "));
	alt.Append(a);
	iFont->drawText3D(alt, -90, -60, -400);
	*/
}

void CNightvision::renderBackground() {
	glDisable(GL_BLEND);
	glDepthMask(GL_FALSE);
	queryInputLib(2, iCameraFrame, _L("")); // Query the camera
	if (iCameraFrame->Count() > 0) {
		glGenTextures(1, &iViewFinderTex);
		glBindTexture(GL_TEXTURE_2D, iViewFinderTex);
		(*iCameraFrame)[0]->LockHeap();
		TUint8* aCamFrameData = (TUint8*) (*iCameraFrame)[0]->DataAddress();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 128, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, aCamFrameData);
		(*iCameraFrame)[0]->UnlockHeap();
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	
	glTranslatex(0, 0, -80 << 16);
	glScalex(20 << 16, 20 << 16, 20 << 16);
	
	glVertexPointer(3, GL_FLOAT, 0, objVertexdata);
	glTexCoordPointer(2, GL_BYTE, 0, objTexdata);
	glBindTexture(GL_TEXTURE_2D, iViewFinderTex);
	glDrawElements(GL_TRIANGLES, NUM_FACES_POLY * 3,
			GL_UNSIGNED_BYTE, objFacedataPoly);
	
	glDeleteTextures(1, &iViewFinderTex);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
}

/* Register input libraries */
void CNightvision::registerInput(IInput* in) {
	iInputLib->AppendL(in);
}

/* handle user input */
void CNightvision::handleInput(TUint key, TBool pressed) {
	iKey = key;
	iKeyPressed = pressed;
}

void CNightvision::render(TInt frm) {
	iFrame = frm;
	renderLoop(iFrame);
}

// leaves
void CNightvision::queryInputLib(TInt aInputNum, TAny * aOut,
		const TDesC& aQuery) {
	if (aInputNum + 1 > iInputLib->Count()) {
		return;
	}
	(*iInputLib)[aInputNum]->query(aOut, aQuery);
}
