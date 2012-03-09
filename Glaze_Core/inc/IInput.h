/*
 * IInput.h
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#ifndef IINPUT_H_
#define IINPUT_H_

#include <e32base.h>    // CBase

class IInput {
public:
	virtual void query(TAny *, const TDesC&) = 0;
	virtual void cleanup() = 0;
};

#endif /* IINPUT_H_ */
