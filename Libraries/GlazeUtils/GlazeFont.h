/*
============================================================================
 Name        : GlazeFont.h
 Author      : Roberto Sousa
 Version     :
 Copyright   : Your copyright notice
 Description : GlazeFont.h - header file
============================================================================
*/

#ifndef GLAZEFONT_H
#define GLAZEFONT_H

// INCLUDES
#include "Utils3d.h" // Utilities (texmanager, textures etc.)
#include <GLES/gl.h> // OpenGL ES header file
#include <e32base.h>  // CBase, link against euser.lib

// CLASS DECLARATION

class CGlazeFont
{
public:
	static CGlazeFont* NewL( TUint fontWidth, TUint fontHeight, 
							TUint screenWidth, TUint screenHeight,
							TUint aTexWidth, TUint aTexHeight);
	virtual ~CGlazeFont();

protected: 
    CGlazeFont( TUint fontWidth, TUint fontHeight, 
				TUint screenWidth, TUint screenHeight,
				TUint aTexWidth, TUint aTexHeight);
    void ConstructL( void );

public:
	void fontTex( TTexture texture );
	void drawText2D(const TDesC& text, TInt xpos, TInt ypos);
	void drawText3D(const TDesC& text, TInt xpos, TInt ypos, TInt zpos);
	void setScale(TReal, TReal, TReal);

private:
	void set2D();
	void unset2D();
	void drawText(const TDesC& text, TInt xpos, TInt ypos, TInt zpos);
	
private:
	TTexture fontTexture;
	TUint fontHeight;
	TUint fontWidth;
	TUint screenHeight;
	TUint screenWidth;
	TUint iTexWidth;
	TUint iTexHeight;
	TReal *fontScale;
	GLbyte *vertex;
	GLbyte *texcoord;
};

#endif // GLAZEFONT_H

// End of File
