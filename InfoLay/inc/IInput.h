#ifndef __IINPUT_H__
#define __IINPUT_H__


#include <e32base.h>    // CBase

class IInput {
public:
	virtual void query(TAny *, const TDesC&) = 0;
	virtual void cleanup() = 0;
};

#endif
