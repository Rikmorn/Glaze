/*
 * IVisualization.h
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#ifndef IVISUALIZATION_H_
#define IVISUALIZATION_H_

#include "IInput.h"
#include <e32base.h>    // CBase

class IVisualization {
public:
	virtual void render(TInt) = 0;
	virtual TBool ready() = 0;
	virtual void execute(TUint, TUint) = 0;
	virtual void registerInput(IInput *) = 0;
	virtual void cleanup() = 0;
	virtual void setScreenSize(TUint, TUint) = 0;
	virtual void handleInput(TUint, TBool) = 0;
};

#endif /* IVISUALIZATION_H_ */
