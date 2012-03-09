/*
 ============================================================================
 Name		: Glaze_CoreAppView.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __GLAZE_COREAPPVIEW_h__
#define __GLAZE_COREAPPVIEW_h__

// INCLUDES
#include <coecntrl.h>
#include <akndef.h>
#include "Core.h"

// CLASS DECLARATION
class CGlaze_CoreAppView : public CCoeControl
	{
public:

	static CGlaze_CoreAppView* NewL(const TRect& aRect);

	static CGlaze_CoreAppView* NewLC(const TRect& aRect);

	virtual ~CGlaze_CoreAppView();

public:

	virtual void SizeChanged();

	virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	
	void HandleInput(TUint key, TBool pressed);
	
	void Draw(const TRect& aRect) const;
    

private:
	void ConstructL(const TRect& aRect);

	CGlaze_CoreAppView();
	
private:
	CGlazeCore* iCore;

	};

#endif // __GLAZE_COREAPPVIEW_h__
// End of File
