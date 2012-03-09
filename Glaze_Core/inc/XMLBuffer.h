/*
 * XMLBuffer.h
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#ifndef XMLBUFFER_H_
#define XMLBUFFER_H_

#include "VisualizationLib.h"
#include <e32std.h>
#include <f32file.h>

class XMLBuffer {
public:
	static XMLBuffer* NewL();
	virtual ~XMLBuffer();

protected: 
    XMLBuffer();
    void ConstructL( void );

public:
	void Parse(const TDesC&);
	void addItem(const TDesC&);
	TPtrC getVName(TInt);
	TPtrC getIName(TInt, TInt);
	TPtrC getFName();
	TInt isTag(const TDesC&);
	void currentTag(TInt);
	TInt getNumLibs();
	TInt getNumVLibs();
	TInt getNumILibs(TInt);

private:
	void addVName(const TDesC&);
	void addVLibrary(const TDesC&);
	void addILibrary(const TDesC&);
	void addFLibrary(const TDesC&);
	void readFile(const TDesC&);

public:
	TBuf<100> iFront;
	TBuf<100> iFrontLib;
	RArray< TBuf<100> > iVisNames;
	RPointerArray<CVisualizationLib> *iVis;
	RArray< TBuf<100> > iSearchTags;
	TInt iIndex;
	TInt iTagIndex;
};

#endif /* XMLBUFFER_H_ */
