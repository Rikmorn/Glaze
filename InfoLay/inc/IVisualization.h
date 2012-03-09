#ifndef __IVISUALIZATION_H__
#define __IVISUALIZATION_H__


#include <e32base.h>    // CBase

#include "IInput.h"

class IVisualization {
public:
	virtual void render(TInt) = 0;
	virtual TBool ready() = 0;
	virtual void execute(TUint, TUint) = 0;
	virtual void registerInput(IInput *) = 0;
	virtual void cleanup() = 0;
	virtual void setScreenSize(TInt, TInt) = 0;
	virtual void handleInput(TUint, TBool) = 0;
};


#endif
