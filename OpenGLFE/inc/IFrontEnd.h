/*
 * IFrontEnd.h
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#ifndef IFRONTEND_H_
#define IFRONTEND_H_

#include <e32base.h>    // CBase

class IFrontEnd {
public:
	virtual void cleanup() = 0;
	virtual TInt getScreenResolutionX() = 0;
	virtual TInt getScreenResolutionY() = 0;
	virtual void swapBuffers() = 0;
	virtual void clearDevice() = 0;
};

#endif /* IFRONTEND_H_ */
