/*
============================================================================
 Name        : OpenGLFEDll.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : OpenGLFEDll.h - COpenGLFEDll class header
               Defines the API for OpenGLFEDll.dll
============================================================================
*/

#ifndef __OPENGLFEDLL_H__
#define __OPENGLFEDLL_H__

// INCLUDES
#include "IFrontEnd.h"

#include <GLES/egl.h>
#include <coecntrl.h>
#include <e32base.h>    // CBase

#define FRUSTUM_LEFT   -1.f     //left vertical clipping plane
#define FRUSTUM_RIGHT   1.f     //right vertical clipping plane
#define FRUSTUM_BOTTOM -1.f     //bottom horizontal clipping plane
#define FRUSTUM_TOP     1.f     //top horizontal clipping plane
#define FRUSTUM_NEAR    3.f     //near depth clipping plane
#define FRUSTUM_FAR  1000.f     //far depth clipping plane

class COpenGL : public IFrontEnd, CCoeControl {
public:
	IMPORT_C static IFrontEnd* NewL();
	COpenGL();
	virtual void cleanup();
	virtual void swapBuffers();
	virtual void clearDevice();
	virtual TInt getScreenResolutionX();
	virtual TInt getScreenResolutionY();

private:
	TInt iScreenX;
	TInt iScreenY;
    EGLDisplay iEglDisplay;
    EGLConfig iConfig;
    EGLSurface iEglSurface;
    EGLContext iEglContext;
};

#endif  // __OPENGLFEDLL_H__


