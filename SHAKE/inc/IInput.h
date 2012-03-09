/*
============================================================================
 Name        : IInput.h
 Author      : Roberto Sousa
 Version     : 0.1
 Copyright   : 
 Description : IInput.h - Interface with methods to be called by whatever
				exterior component using this dll.
============================================================================
*/

#ifndef __IINPUT_H__
#define __IINPUT_H__

#include <e32base.h>    // CBase

class IInput {
public:
	virtual void query(TAny *, const TDesC&) = 0;
	virtual void cleanup() = 0;
};

#endif
