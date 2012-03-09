/*
============================================================================
 Name        : GlazeTextures.h
 Author      : Roberto Sousa
 Version     :
 Copyright   : Your copyright notice
 Description : GlazeTextures.h - header file
============================================================================
*/

#ifndef GLAZETEXTURES_H
#define GLAZETEXTURES_H

#include "Utils3d.h" // Utilities (texmanager, textures etc.)
#include "GlazeCommon.h"
#include <GLES/gl.h> // OpenGL ES header file
#include <e32base.h>  // CBase, link against euser.lib
#include <e32cmn.h>

class CGlazeTextures : public CFiniteStateMachine, public MTextureLoadingListener
{
public:
	static CGlazeTextures* NewL (TFileName& aFileName, TUint aWidth, TUint aHeight );
	virtual ~CGlazeTextures();

protected:
	CGlazeTextures( TFileName& aFileName, TUint aWidth, TUint aHeight );
	void ConstructL();

	void OnEnterStateL( TInt aState );
	void OnStartLoadingTexturesL();
	void OnEndLoadingTexturesL();

public:
	void addTexture(TBuf<64> name);
	void loadTextures();
	TTexture getTexture(TUint index);

public:
	enum
    	{
    	ELoadingTextures,
    	EDone
    	};

private:
	RArray<TTexture> * textures;
	CTextureManager * iTextureManager;
};

#endif // GLAZETEXTURES_H

// End of File
