/*
 * ShakeParser.h
 *
 *  Created on: 23/Abr/2009
 *      Author: Daemoniorum
 */

#ifndef SHAKEPARSER_H_
#define SHAKEPARSER_H_

#include <e32std.h>
#include <e32base.h>

#define NUMSAMPLES 5

_LIT(KCommand,"HED");

class CShakeParser{
public:
	static CShakeParser* NewL();
	virtual ~CShakeParser();

protected: 
	CShakeParser();
    void ConstructL( void );
    
public:
	void Parse(const TDesC& aData);
	
private:
	void handleToken();
	TInt currentToken(const TDesC& aData);
	void parseToken();
	void RollingAVGFilter(TInt aOri);
	
public:
	TReal iOrientation;
	
private:
	TBuf<100> iBuffer;
	TInt iOriBuffer[NUMSAMPLES];
	TInt iOriIndex;
	TInt iTokenNum;
	TBool iDolarFound;
	TInt iCommand;
	TInt iPosition;
};

#endif /* SHAKEPARSER_H_ */
