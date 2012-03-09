/*
 * VisualizationLib.h
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#ifndef VISUALIZATIONLIB_H_
#define VISUALIZATIONLIB_H_

#include <e32std.h>

class CVisualizationLib {
public:
	static CVisualizationLib* NewL(const TDesC& aName);
	virtual ~CVisualizationLib();

protected: 
    CVisualizationLib(const TDesC& aName);
    void ConstructL( void );

public:
	void addInputLib(const TDesC& aName);
	void numStates(TUint aNum);
	TPtrC getVName();
	TPtrC getIName(TUint aPos);
	TUint getNumInputs();

private:
	TBuf<100> iName;
	RArray< TBuf<100> > iInputName;
	TUint iNum;
};

#endif /* VISUALIZATIONLIB_H_ */
