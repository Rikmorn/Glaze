/*
============================================================================
 Name        : CInputDll from InputDll.h
 Author      : Roberto Sousa
 Version     : 0.1
 Copyright   : 
 Description : CInput DLL source
============================================================================
*/

//  Include Files  

#include "CameraInput.h"			// CInput
#include "CameraInput.pan"      	// panic codes

// DLL constructor, no more can be added to the method
EXPORT_C IInput* CCameraInput::NewL(){
	return new CCameraInput();
}

/* CInput constructor */
CCameraInput::CCameraInput() {
	iReady = EFalse;
	iCamera = CCamera::NewL(*this, 0);	
	iCamera->Reserve();	
	iCaptureBitmap = new (ELeave) CFbsBitmap();
}

/* 
	Query method. the objective of this method is to receive 
	a query from an exterior component analyse that query
	and provide a responce, that response must be saved in
	out.
*/
void CCameraInput::query(TAny* out, const TDesC& query){ 
	if (iReady){
		((RPointerArray<CFbsBitmap>*) out)->Reset();
		((RPointerArray<CFbsBitmap>*) out)->Append(iCaptureBitmap);
	}
}

/*
	Cleanup method this function will be called right before 
	the deletion of Input, so this method can be seen as the
	destructor of the DLL.
*/
void CCameraInput::cleanup(){
	delete iCaptureBitmap;
}

// called when camera reservation is complete
void CCameraInput::ReserveComplete(TInt aError){
	if  (aError  ==  KErrNone) {
		iCamera->PowerOn();
	}
	else
	{
		_LIT(KCamera, "Cam Reserve Fail");
		User::Panic( KCamera, 0 );
	}
}

// called when power up is complete
void CCameraInput::PowerOnComplete(TInt aError){
	if  (aError  ==  KErrNone) {
		TSize size;
		//size.iWidth = iScreenWidth;
		//size.iHeight = iScreenHeight;
		size.iWidth = 256;
		size.iHeight = 256;
		iCamera->StartViewFinderBitmapsL(size);
	}
	else
	{
		_LIT(KCamera, 	"Cam PowerOn Fail");
		User::Panic( KCamera, 0 );
	}
}

// called when viewfinder frame is ready for processing
void CCameraInput::ViewFinderFrameReady(CFbsBitmap& aFrame){
	TInt iWidth,iHeight;
	iWidth=aFrame.SizeInPixels().iWidth;
	iHeight=aFrame.SizeInPixels().iHeight;
	
	TBuf8<256*4> aBuf;
	TPoint Pt(0,0); 
	// convert frame to something OpenGL ES ready
	iCaptureBitmap->Create(aFrame.SizeInPixels(),EColor16MU);
	iCaptureBitmap->LockHeap();
	
	for (TInt i = 0;i < iHeight ; i++) {
	   Pt.iY=i;
	   aFrame.GetScanLine(aBuf,Pt,iWidth,EColor16MU);
	   iCaptureBitmap->SetScanLine(aBuf,i);
	}
	TUint8* data;
	TUint8 temp;
	//iCaptureBitmap->LockHeap();
	data = (TUint8*)iCaptureBitmap->DataAddress();
	TInt framesize = iCaptureBitmap->SizeInPixels().iHeight*iCaptureBitmap->SizeInPixels().iWidth;
	for (TInt i = 0;i < framesize*4-4 ; i=i+4) {
		temp = data[i];
		data[i] = data[i+2];
		data[i+2] = temp;
	}
	iCaptureBitmap->UnlockHeap();
	iReady = ETrue;
}

// these two methods are of no use for now
void CCameraInput::ImageReady(CFbsBitmap* aBitmap,HBufC8* aData,TInt aError){
	
}
void CCameraInput::FrameBufferReady(MFrameBuffer* aFrameBuffer,TInt aError){
	
}
