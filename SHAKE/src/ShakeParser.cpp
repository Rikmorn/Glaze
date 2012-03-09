/*
 * ShakeParser.cpp
 *
 *  Created on: 23/Abr/2009
 *      Author: Daemoniorum
 */

#include "ShakeParser.h"

CShakeParser* CShakeParser::NewL() {
	CShakeParser* self = new (ELeave) CShakeParser();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

CShakeParser::CShakeParser() {
	iOrientation = 0;
	iOriIndex = 0;
}

CShakeParser::~CShakeParser() {
}

void CShakeParser::ConstructL(void) {
}

void CShakeParser::Parse(const TDesC& aData) {
	TInt size = aData.Length();
	for (iPosition = 0; /*aData[iPosition] != '\n'*/iPosition < size; iPosition++) {
		if (aData[iPosition] == '$' || aData[iPosition] == ',') {
			if (aData[iPosition] == '$') {
				//iBuffer.Zero();
				iDolarFound = ETrue;
			} else
				handleToken();
		} else {
			// check to see if the buffer is full
			if (iBuffer.Length() < 100){ // leave
				iBuffer.Append(aData[iPosition]);
			}
		}
		
	}
}

void CShakeParser::handleToken() {
	if (iBuffer.Length() == 0)
		return;
	if (iDolarFound) {
		iTokenNum = 0;
		iCommand = currentToken(iBuffer);
		iDolarFound = EFalse;
	} else {
		parseToken();
		iTokenNum++;
	}
	iBuffer.Zero();
}

TInt CShakeParser::currentToken(const TDesC& aData) {
	return (aData.Find(KCommand) >= 0) ? 0 : -1;
}

// parse the token according to iCommand and iTokenNum
void CShakeParser::parseToken() {
	if (iBuffer.Length() == 0) {
		return;
	}
	if (iCommand == 0 && iTokenNum == 0) {
		TInt tempOri;
		TLex iLex(iBuffer);
		iLex.Val(iOrientation);
		//iLex.Val(tempOri);
		//RollingAVGFilter(tempOri);
	}
}

void CShakeParser::RollingAVGFilter(TInt aOri) {
	iOriBuffer[iOriIndex % NUMSAMPLES] = aOri;
	if (iOriIndex >= NUMSAMPLES - 1) {
		TInt temp = 0;
		for (TInt i = 0; i < NUMSAMPLES; i++) {
			temp += iOriBuffer[i];
		}
		iOrientation = (TReal)temp / (TReal)NUMSAMPLES;
	}
	iOriIndex++;
}
