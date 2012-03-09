/*
============================================================================
 Name        : Input.h
 Author      : Roberto Sousa
 Version     : 0.1
 Copyright   : 
 Description : Input.h - CInput class header, defines the entry point for
				dll. Add any methods or classes needed to read the raw data
============================================================================
*/

#ifndef __INPUT_H__
#define __INPUT_H__

#include "IInput.h"
#include <e32base.h>    // CBase
#include <ecam.h>
#include "fbs.h"

class CCameraInput : public IInput, MCameraObserver {
public:
	IMPORT_C static IInput* NewL();
	CCameraInput();

	virtual void query(TAny *, const TDesC&);
	virtual void cleanup();
	
private:
	virtual void ReserveComplete(TInt aError);
	virtual void PowerOnComplete(TInt aError);
	virtual void ViewFinderFrameReady(CFbsBitmap& aFrame);
	virtual void ImageReady(CFbsBitmap* aBitmap,HBufC8* aData,TInt aError);
	virtual void FrameBufferReady(MFrameBuffer* aFrameBuffer,TInt aError);
	
private:
	CFbsBitmap* iCaptureBitmap;
	CCamera* iCamera;
	TBool iReady;
};

#endif  // __INPUTDLL_H__


