/*
 * Core.h
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#ifndef CORE_H_
#define CORE_H_

#include <e32std.h>
#include <f32file.h>
#include "XMLBuffer.h"
#include "LocalLibrary.h"

class CGlazeCore {
public:
	static CGlazeCore* NewL();
	virtual ~CGlazeCore();

protected: 
    CGlazeCore();
    void ConstructL( void );

public:
	void setScreenSize(TInt, TInt);
	void handleKey(TUint, TBool);
	static TInt DrawCallBack( TAny* aInstance );
	void renderLoop(TInt);

private:
	CLocalLibrary* iLibrary;
	XMLBuffer* iXml;
	RLibrary front;
	TBool iReady;
	TInt iFrame;
	CPeriodic* iPeriodic;
};

#endif /* CORE_H_ */
