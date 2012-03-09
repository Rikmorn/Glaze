/*
============================================================================
 Name        : GlazeTextures.cpp
 Author      : Roberto Sousa
 Version     :
 Copyright   : Your copyright notice
 Description : GlazeTextures.cpp - source file
============================================================================
*/

// INCLUDE FILES
// Class include
#include "GlazeTextures.h"
// System includes
//#include <e32base.h>  // For CBase, link against euser.lib

// ================= MEMBER FUNCTIONS =======================
CGlazeTextures* CGlazeTextures::NewL( TFileName& aFileName, TUint aWidth, TUint aHeight )
{
    CGlazeTextures* self = new (ELeave) CGlazeTextures( aFileName, aWidth, aHeight );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
}

CGlazeTextures::CGlazeTextures( TFileName& aFileName, TUint aWidth, TUint aHeight )
{
	iTextureManager = CTextureManager::NewL ( aFileName, aWidth, aHeight,
											FRUSTUM_TOP, FRUSTUM_BOTTOM, 
											FRUSTUM_RIGHT, FRUSTUM_LEFT, FRUSTUM_NEAR,
                                            this );
	textures = new (ELeave) RArray<TTexture>;
}

CGlazeTextures::~CGlazeTextures() 
{
	textures->Reset();
	textures->Close();
	delete textures;
	delete iTextureManager;
}

void CGlazeTextures::ConstructL( void ) { }

void CGlazeTextures::OnStartLoadingTexturesL()
{
	SetStateL( ELoadingTextures );
}

void CGlazeTextures::OnEndLoadingTexturesL()
{
	if ( GetState() == ELoadingTextures )
	{
		SetStateL( EDone );
	}
}

void CGlazeTextures::OnEnterStateL( TInt ) { }

void CGlazeTextures::addTexture(TBuf<64> name)
{
	TTexture temp;
	textures->Append(temp);
	iTextureManager->RequestToLoad( name, &(*textures)[textures->Count()-1] );	
}

void CGlazeTextures::loadTextures()
{
	iTextureManager->DoLoadL();
}

TTexture CGlazeTextures::getTexture(TUint index)
{
	return (*textures)[index];
}

// End of File
