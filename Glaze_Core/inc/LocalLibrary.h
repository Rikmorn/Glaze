/*
 * LocalLibrary.h
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#ifndef LOCALLIBRARY_H_
#define LOCALLIBRARY_H_

#include "LibraryManager.h"

//#include <e32std.h>

class CLocalLibrary: public CLibraryManager {
public:
	static CLocalLibrary* NewL(TUint);
	virtual ~CLocalLibrary();

protected: 
    CLocalLibrary(TUint);
    void ConstructL( void );

public:
	void addFrontEndLib(const TDesC &);
	void addVisualizationLib(const TDesC &);
	void addInputLib(const TDesC &);

private:
	TLibraryFunction OpenLib(const TDesC &);

private:
	TUint iCurrentLib;
	TInt iCurrentVis;
};

#endif /* LOCALLIBRARY_H_ */
