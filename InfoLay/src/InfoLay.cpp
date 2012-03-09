/*
 ============================================================================
 Name        : TargetVisualization.cpp
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : TargetVisualization.cpp - source file
 ============================================================================
 */

#include "InfoLay.h"

EXPORT_C IVisualization* CInfoLay::NewL() {
	return new CInfoLay();
}

CInfoLay::CInfoLay() {
	iInputLib = new (ELeave) RPointerArray<IInput> ;
	iGPSData = new (ELeave) RArray<TReal64> ;
	iShakeData = new (ELeave) RArray<TReal> ;
	iCameraFrame = new (ELeave) RPointerArray<CFbsBitmap> ;
	iKey = 0;
	iFrame = 0;
	iHeading = 1.0;
	iKeyPressed = EFalse;
	iStopIn = EFalse;
	positionBuffer = new TInt[6];
	for (TInt i = 0; i < 6; i++) {
		positionBuffer[i] = 0;
	}
	round = ETrue;
}

void CInfoLay::execute(TUint iW, TUint iH) {
	TFileName iFile = _L("c:\\data\\others\\glaze\\textures\\");
	iTextures = CGlazeTextures::NewL(iFile, iW, iH);
	iFont = CGlazeFont::NewL(8, 13, iW, iH, TEX_WIDTH, TEX_HEIGHT);
	iAnimManager = CGlazeAnimationManager::NewL();
	iArrows = CGlazeArrow::NewL();
	iGui = CInfoLayer::NewL();
	iMeshList = CMeshList::NewL(3);
	/*
	 * Home positions
	 * x = 32647400;
	 * y = 39;
	 * z = -16844000;
	 * x = 32648900;
	 * y = 39
	 * z = -16843130
	 */
	// UMA Cantina - 32.658020, -16.924250
	/* initialize object info */
	iMeshList->setAreaName(_L("Funchal"));
	iMeshList->setAreaInfo(0,_L("Informacao geral estilo wiki sobre o Funchal."));
	iMeshList->setAreaInfo(1,_L("dd/mm/yy - informacao neste estilo sobre eventos proximos relacionados com o Funchal."));
	iMeshList->setAreaInfo(2,_L("Tag cloud sobre coisas que as pessoas que viram este alvo deixaram para os outros verem."));
	iMeshList->setAreaInfo(3,_L("Outras informacoes de interese."));
	
	//32.658950, -16.924300
	iMeshList->setPosition(0, 32658950,39,-16924300);
	iMeshList->setName(0, _L("Campus"));
	iMeshList->setInfo(0,0,_L("Informacao geral estilo wiki sobre o campus."));
	iMeshList->setInfo(0,1,_L("01/07/09 - Fim das aulas, informacao neste estilo sobre eventos proximos relacionados com o campus"));
	iMeshList->setInfo(0,2,_L("Tag cloud sobre coisas que as pessoas que viram este alvo deixaram para os outros verem."));
	iMeshList->setInfo(0,3,_L("Outras informacoes de interese."));
	
	//32.659970, -16.926300
	iMeshList->setPosition(1, 32659970,39,-16926300);
	iMeshList->setName(1, _L("Tecnopolo"));
	iMeshList->setInfo(1,0,_L("Informacao geral estilo wiki sobre o tecnopolo."));
	iMeshList->setInfo(1,1,_L("dd/mm/yy - informacao neste estilo sobre eventos proximos relacionados com o tecnopolo."));
	iMeshList->setInfo(1,2,_L("Tag cloud sobre coisas que as pessoas que viram este alvo deixaram para os outros verem."));
	iMeshList->setInfo(1,3,_L("Outras informacoes de interese."));
	
	// 32.658150, -16.924300
	iMeshList->setPosition(2, 32658150,39,-16924300);
	iMeshList->setName(2, _L("Cantina"));
	iMeshList->setInfo(2,0,_L("Informacao geral estilo wiki sobre o tecnopolo."));
	iMeshList->setInfo(2,1,_L("dd/mm/yy - informacao neste estilo sobre eventos proximos relacionados com a cantina."));
	iMeshList->setInfo(2,2,_L("Tag cloud sobre coisas que as pessoas que viram este alvo deixaram para os outros verem."));
	iMeshList->setInfo(2,3,_L("Outras informacoes de interese."));
	
	iArrows->createCoords(iMeshList->meshNum());
	initVisualization(iW, iH);
}

/* cleanup after yourself */
void CInfoLay::cleanup() {
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
	delete positionBuffer;
	delete iArrows;
	delete iMeshList;
}

/* change screen size, this is called in during OpenGL initialization
 or if the screen needs to change size for some reason */
void CInfoLay::setScreenSize(TInt iWidth, TInt iHeight) {
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
void CInfoLay::initVisualization(TUint w, TUint h) {
	iWidth = w;
	iHeight = h;
	
	// initialize info on all meshes
	// add meshes to the animation manager
	
	for (TInt i=0; i<iMeshList->meshNum(); i++){
		iAnimManager->addMesh(iMeshList->getMesh(i));
	}
	iAnimManager->addMesh(iGui);

	iTextures->addTexture(_L("font.png"));
	iTextures->addTexture(_L("arrow.png"));
	iTextures->addTexture(_L("target.png"));
	iTextures->addTexture(_L("choice.png"));
	iTextures->addTexture(_L("information_gui.png"));
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

void CInfoLay::assignTextures() {
	iFont->fontTex(iTextures->getTexture(0));
	for (TInt i = 0; i<iMeshList->meshNum();i++){
		iMeshList->setTexture(i,iTextures->getTexture(2));
		iMeshList->setFontTexture(i,iTextures->getTexture(0));
	}
	iMeshList->setAreaFontTexture(iTextures->getTexture(0));
}

void CInfoLay::updateUserInput() {
	if (iKeyPressed) {
		switch (iKey) {
		case 14: // left arrow
			if (!iStopIn) {
				// check if gui is active
				iAnimManager->playAnimation(3, 0, 100);
				iStopIn = ETrue;
			}
			break;
		case 15: // right arrow
			if (!iStopIn) {
				iAnimManager->playAnimation(3, 1, 100);
				iStopIn = ETrue;
			}
			break;
		case 16: //up arrow
			if (!iStopIn){
				TBool tg = iGui->toggleGUI();
				if (tg){
					iAnimManager->playAnimation(3, 2, 100);
				} else {
					iAnimManager->playAnimation(3, 3, 100);
				}
				iStopIn = ETrue;
			}
			break;
		case 17: // down arrow

			break;			
		}
	} else {
		iStopIn = EFalse;
	}
}

TBool CInfoLay::ready() {
	if (iTextures->GetState() == CGlazeTextures::EDone) {
		assignTextures();
		return ETrue;
	}
	return EFalse;
}

/* Target visualization's render cycle */
void CInfoLay::renderLoop(TInt aFrame) {
	updateUserInput();
	queryInputLib(0, iGPSData, _L(""));
	queryInputLib(1, iShakeData, _L(""));

	TInt userx = (TInt) ((*iGPSData)[0] * 1000000);
	TInt userz = (TInt) ((*iGPSData)[1] * 1000000);
	TInt usery = (TInt) (*iGPSData)[2]; // geoid
	
	iHeading = (*iShakeData)[0];
	
	for (TInt i = 0; i<iMeshList->meshNum();i++){
		/* update compass */
		TReal objX, objY, objZ;
		iMeshList->getCurrentPosition(i, objX, objY, objZ);
		iArrows->updateObjCoords(i, objX, objY, objZ);
		/* update animation */
		if (iMeshList->meshMoved(i, userx,usery,userz)){
			iAnimManager->playAnimation(i, 0, 900);
		}
	}
	
	// render viewfinder 
	glLoadIdentity();
	renderBackground();
	
	//TReal scenerot = 360.0 - iPanX;
	glLoadIdentity();
	glRotatef(0.0, 1, 0, 0); //Look up and down
	glRotatef(iHeading + 90, 0, 1, 0); // look right and left
	//glRotatef(scenerot, 0, 1, 0);
	//glTranslatef(posx*5, 0, posz*5); // manual camera translating
	
	iAnimManager->updateAnimations();

	iMeshList->renderMeshes();
	
	iArrows->renderArrows(iTextures, iHeading + 90);
	
	TInt target = iMeshList->closestTarget(iHeading);
	
	// print target name
	glLoadIdentity();
	iFont->setScale(1, 1, 1);
	TBuf<100> ori;

	ori.Append(iMeshList->getMeshName(target));
	iFont->drawText3D(ori, -80, 100, -400);

	
	// render 2DGUI, get iMeshList for all the text for the selected mesh
	iGui->renderGUI(iTextures, iMeshList, target);
}

void CInfoLay::renderBackground() {
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
	
	glTranslatex(0, 0, -300 << 16);
		
	glVertexPointer(3, GL_FLOAT, 0, camVert);
	glTexCoordPointer(2, GL_BYTE, 0, camTex);
	glBindTexture(GL_TEXTURE_2D, iViewFinderTex);
	glDrawElements(GL_TRIANGLES, NUM_FACES_POLY * 3,
			GL_UNSIGNED_BYTE, camFace);
	
	glDeleteTextures(1, &iViewFinderTex);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
}

/* Register input libraries */
void CInfoLay::registerInput(IInput* in) {
	iInputLib->AppendL(in);
}

/* handle user input */
void CInfoLay::handleInput(TUint key, TBool pressed) {
	iKey = key;
	iKeyPressed = pressed;
}

void CInfoLay::render(TInt frm) {
	iFrame = frm;
	renderLoop(iFrame);
}

// leaves
void CInfoLay::queryInputLib(TInt aInputNum, TAny * aOut,
		const TDesC& aQuery) {
	if (aInputNum + 1 > iInputLib->Count()) {
		return;
	}
	(*iInputLib)[aInputNum]->query(aOut, aQuery);
}
