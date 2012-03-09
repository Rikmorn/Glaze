/*
============================================================================
 Name        : GlazeFont.cpp
 Author      : Roberto Sousa
 Version     :
 Copyright   : Your copyright notice
 Description : GlazeFont.cpp - source file
============================================================================
*/

// INCLUDE FILES
// Class include
#include "GlazeFont.h"
// System includes
//#include <e32base.h>  // For CBase, link against euser.lib


// ================= MEMBER FUNCTIONS =======================

CGlazeFont* CGlazeFont::NewL( TUint fontWidth, TUint fontHeight, 
							TUint screenWidth, TUint screenHeight,
							TUint aTexWidth, TUint aTexHeight)
{
    CGlazeFont* self = new (ELeave) CGlazeFont( fontWidth, fontHeight, screenWidth, screenHeight, aTexWidth, aTexHeight);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
}
CGlazeFont::CGlazeFont( TUint fontW, TUint fontH, 
						TUint screenW, TUint screenH,
						TUint aTexWidth, TUint aTexHeight)
{
	fontHeight = fontH;
	fontWidth = fontW;
	screenHeight = screenH;
	screenWidth = screenW;
	iTexWidth = aTexWidth;
	iTexHeight = aTexHeight;
	fontScale = new TReal[3];
}

CGlazeFont::~CGlazeFont() 
{
	delete fontScale;
}

void CGlazeFont::ConstructL( void ) { }

void CGlazeFont::fontTex( TTexture texture )
{
	fontTexture = texture;
}

// gotta draw this in an ortho matrix
void CGlazeFont::drawText2D( const TDesC& text, TInt xpos, TInt ypos){
	set2D();
	drawText(text, xpos, ypos, 0);
	unset2D();
}

void CGlazeFont::drawText3D(const TDesC& text, TInt xpos, TInt ypos, TInt zpos){
	drawText(text, xpos, ypos, zpos);
}

void CGlazeFont::drawText(const TDesC& text, TInt xpos, TInt ypos, TInt zpos){
	GLfixed vertex[12] = {
		    0,				fontHeight << 16,	1 << 16,
			fontWidth << 16,fontHeight << 16,	1 << 16,
			fontWidth << 16,0,					1 << 16,
			0,				0,					1 << 16
		};
	GLubyte face[] = {
			1,0,3,
			1,3,2
		};
	glVertexPointer( 3, GL_FIXED, 0, vertex );

	/*  for each char in the string we are going to get it's ascii value
		and calculate it's position in the texture, we'll then sample
		the texture and render the polygon with that letter */
	
	for (TInt i = 0; i < text.Length(); i++)
	{
		TUint c = text[i] - 32;
		// ignore spaces
		if (c==0) continue;
		TUint chardiv = iTexWidth / fontWidth;
		TUint line = c / chardiv;
		TUint column = c - 1 - ( chardiv * line);
		// 256 because texture mapping coordenates go from -128 to 127
		TUint texpadW = fontWidth * iTexWidth / 256;
		TUint texpadH = fontHeight * iTexHeight / 256;
		GLbyte tuv[8] = {
					(texpadW * column) - 128, (texpadH * line) - 128, // UL
					(texpadW * column) - 128 + texpadW - 1, (texpadH * line) - 128, // UR
					(texpadW * column) - 128 + texpadW - 1, (texpadH * line) - 128 + texpadH, // LR
					(texpadW * column) - 128, (texpadH * line) - 128 + texpadH // LL
				};
		glTexCoordPointer( 2, GL_BYTE, 0, tuv );
		glLoadIdentity();
		glTranslatex( (TInt) (xpos + (fontWidth * fontScale[0] * i)) << 16, ypos << 16,  zpos << 16);
		glScalef( fontScale[0], fontScale[1], fontScale[2] );
		glBindTexture( GL_TEXTURE_2D, fontTexture.iID );
		glTexEnvx( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, face );
	}
}

void CGlazeFont::set2D(){
	glDisable(GL_DEPTH_TEST);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrthox( 0, screenWidth << 16 , 0, screenHeight << 16 , -1 << 16 , 1 << 16  );
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void CGlazeFont::unset2D(){
	//glDisableClientState( GL_COLOR_ARRAY );
	//glDisable(GL_COLOR_MATERIAL);
	//glEnable(GL_TEXTURE_2D);
	// Restore old Matrixes
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void CGlazeFont::setScale(TReal x, TReal y, TReal z){
	fontScale[0] = x;
	fontScale[1] = y;
	fontScale[2] = z;
}



// End of File
