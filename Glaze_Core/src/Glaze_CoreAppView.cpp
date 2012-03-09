/*
 ============================================================================
 Name		: Glaze_CoreAppView.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Application view implementation
 ============================================================================
 */

// INCLUDE FILES
#include <coemain.h>
#include "Glaze_CoreAppView.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CGlaze_CoreAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGlaze_CoreAppView* CGlaze_CoreAppView::NewL(const TRect& aRect) {
	CGlaze_CoreAppView* self = CGlaze_CoreAppView::NewLC(aRect);
	CleanupStack::Pop(self);
	return self;
}

// -----------------------------------------------------------------------------
// CGlaze_CoreAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGlaze_CoreAppView* CGlaze_CoreAppView::NewLC(const TRect& aRect) {
	CGlaze_CoreAppView* self = new (ELeave) CGlaze_CoreAppView;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
}

// -----------------------------------------------------------------------------
// CGlaze_CoreAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CGlaze_CoreAppView::ConstructL(const TRect& aRect) {
	iCore = CGlazeCore::NewL();
}

// -----------------------------------------------------------------------------
// CGlaze_CoreAppView::CGlaze_CoreAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CGlaze_CoreAppView::CGlaze_CoreAppView() {
	// No implementation required
}

// -----------------------------------------------------------------------------
// CGlaze_CoreAppView::~CGlaze_CoreAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CGlaze_CoreAppView::~CGlaze_CoreAppView() {
	if (iCore)
		delete iCore;
}

// -----------------------------------------------------------------------------
// CGlaze_CoreAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CGlaze_CoreAppView::Draw(const TRect& /*aRect*/) const {/*
 // Get the standard graphics context
 CWindowGc& gc = SystemGc();

 // Gets the control's extent
 TRect drawRect(Rect());

 // Clears the screen
 gc.Clear(drawRect);
 */
}

// -----------------------------------------------------------------------------
// CGlaze_CoreAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CGlaze_CoreAppView::SizeChanged() {
	TSize size;
	size = this->Size();
	iCore->setScreenSize(size.iWidth, size.iHeight);
}

// -----------------------------------------------------------------------------
// CGlaze_CoreAppView::HandlePointerEventL()
// Called by framework to handle pointer touch events.
// Note: although this method is compatible with earlier SDKs, 
// it will not be called in SDKs without Touch support.
// -----------------------------------------------------------------------------
//
void CGlaze_CoreAppView::HandlePointerEventL(const TPointerEvent& aPointerEvent) {

	// Call base class HandlePointerEventL()
	CCoeControl::HandlePointerEventL(aPointerEvent);
}

void CGlaze_CoreAppView::HandleInput(TUint key, TBool pressed) {
	iCore->handleKey(key, pressed);
}

// End of File
