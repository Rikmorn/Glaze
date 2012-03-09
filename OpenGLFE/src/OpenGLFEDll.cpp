/*
============================================================================
 Name        : COpenGLFEDll from OpenGLFEDll.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : COpenGLFEDll DLL source
============================================================================
*/

//  Include Files  

#include "OpenGLFEDll.h"	// COpenGLFEDl
#include "OpenGLFEDll.pan"      	// panic codes

//  Member Functions

EXPORT_C IFrontEnd* COpenGL::NewL(){
	return new COpenGL();
}

COpenGL::COpenGL(){
	CreateWindowL();
    SetExtentToWholeScreen(); 
    ActivateL();
 
    iEglDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
    if ( iEglDisplay == NULL ) {
        _LIT(KGetDisplayFailed, "eglGetDisplay failed");
        User::Panic( KGetDisplayFailed, 0 );
    }
    // Initialize display
    if ( eglInitialize( iEglDisplay, NULL, NULL ) == EGL_FALSE ){
        _LIT(KInitializeFailed, "eglInitialize failed");
        User::Panic( KInitializeFailed, 0 );
	}
    EGLConfig *configList = NULL;
    EGLint numOfConfigs = 0;
    EGLint configSize   = 0;
    // Get the number of possible EGLConfigs
    if ( eglGetConfigs( iEglDisplay, configList, configSize, &numOfConfigs ) == EGL_FALSE ){
        _LIT(KGetConfigsFailed, "eglGetConfigs failed");
        User::Panic( KGetConfigsFailed, 0 );
	}
    configSize = numOfConfigs;
    // Allocate memory for the configList
    configList = (EGLConfig*) User::Alloc( sizeof(EGLConfig)*configSize );
    if ( configList == NULL ) {
        _LIT(KConfigAllocFailed, "config alloc failed");
        User::Panic( KConfigAllocFailed, 0 );
	}

    TDisplayMode DMode = Window().DisplayMode();
    TInt BufferSize = 0;
    switch ( DMode ) {
         case(EColor4K):
             BufferSize = 12;
             break;
         case(EColor64K):
             BufferSize = 16;
             break;
         case(EColor16M):
             BufferSize = 24;
             break;
         case(EColor16MU):
		     case(EColor16MA):
             BufferSize = 32;
             break;
         default:
             _LIT(KDModeError, "unsupported displaymode");
             User::Panic( KDModeError, 0 );
             break;
         }

    // Define properties for requesting a non-antialiased window surface
    const EGLint attrib_list[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
		EGL_BUFFER_SIZE,  BufferSize,
        EGL_DEPTH_SIZE,   16,
        EGL_NONE
	};
    if ( eglChooseConfig( iEglDisplay, attrib_list, configList, configSize, &numOfConfigs ) == EGL_FALSE ) {
      _LIT( KChooseConfigFailed, "eglChooseConfig failed" );
      User::Panic( KChooseConfigFailed, 0 );
	}
    if ( numOfConfigs == 0 ) {
      _LIT( KNoConfig, "Can't find the requested config." );
      User::Panic( KNoConfig, 0 );
	}
    iConfig = configList[0];
    delete configList;

    // Create a window where the graphics are blitted
    iEglSurface = eglCreateWindowSurface( iEglDisplay, iConfig, &Window(), NULL );
    if ( iEglSurface == NULL ) {
        _LIT(KCreateWindowSurfaceFailed, "eglCreateWindowSurface failed");
        User::Panic( KCreateWindowSurfaceFailed, 0 );
	}

    // Create a rendering context
    iEglContext = eglCreateContext( iEglDisplay, iConfig, EGL_NO_CONTEXT, NULL );
    if ( iEglContext == NULL ) {
        _LIT(KCreateContextFailed, "eglCreateContext failed");
        User::Panic( KCreateContextFailed, 0 );
	}

    if ( eglMakeCurrent( iEglDisplay, iEglSurface, iEglSurface, iEglContext ) == EGL_FALSE ) {
        _LIT(KMakeCurrentFailed, "eglMakeCurrent failed");
        User::Panic( KMakeCurrentFailed, 0 );
	}
    TSize size = this->Size();
    iScreenX = size.iWidth;
    iScreenY = size.iHeight;
}

void COpenGL::swapBuffers(){
	eglSwapBuffers( iEglDisplay, iEglSurface);
}

void COpenGL::clearDevice(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

TInt COpenGL::getScreenResolutionX(){
	return iScreenX;
}

TInt COpenGL::getScreenResolutionY(){
	return iScreenY;
}

void COpenGL::cleanup(){
	eglMakeCurrent( iEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
    eglDestroySurface( iEglDisplay, iEglSurface );
    eglDestroyContext( iEglDisplay, iEglContext );
    eglTerminate( iEglDisplay );
	CCoeControl::CloseWindow();
	//CCoeControl::~CCoeControl();
}
