/*
 * XMLBuffer.cpp
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#include "XMLBuffer.h"
#include "XMLHelper.h"
#include <e32base.h>

XMLBuffer* XMLBuffer::NewL() {
	XMLBuffer* self = new (ELeave) XMLBuffer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

XMLBuffer::XMLBuffer() {
	iVis = new (ELeave) RPointerArray<CVisualizationLib> ;
	iIndex = -1;
	iSearchTags.AppendL(_L("FE")); //0
	iSearchTags.AppendL(_L("V")); //1
	iSearchTags.AppendL(_L("vname")); //2
	iSearchTags.AppendL(_L("inputlib")); //3
	iSearchTags.AppendL(_L("fname")); //4
}

XMLBuffer::~XMLBuffer() {
	iVis->ResetAndDestroy();
	iVis->Close();
	delete iVis;
	iVisNames.Reset();
	iVisNames.Close();
	iSearchTags.Reset();
	iSearchTags.Close();
}

void XMLBuffer::ConstructL(void) {
}

void XMLBuffer::addVLibrary(const TDesC &aName) {
	CVisualizationLib* lib = CVisualizationLib::NewL(aName);
	iVis->AppendL(lib);
	iIndex++;
}

void XMLBuffer::addILibrary(const TDesC &aName) {
	(*iVis)[iIndex]->addInputLib(aName);
}

TPtrC XMLBuffer::getVName(TInt aPos) {
	return (*iVis)[aPos]->getVName();
}

void XMLBuffer::addVName(const TDesC &aName) {
	TBuf<100> name(aName);
	iVisNames.AppendL(name);
}

TPtrC XMLBuffer::getIName(TInt aPosV, TInt aPosI) {
	return (*iVis)[aPosV]->getIName(aPosI);
}

TInt XMLBuffer::isTag(const TDesC &aTag) {
	for (TInt i = 0; i < iSearchTags.Count(); i++)
		if (aTag == iSearchTags[i])
			return i;
	return -1;
}

void XMLBuffer::currentTag(TInt aIndex) {
	iTagIndex = aIndex;
}

void XMLBuffer::addFLibrary(const TDesC& aName) {
	iFrontLib = aName;
}

TPtrC XMLBuffer::getFName() {
	return iFrontLib;
}

void XMLBuffer::addItem(const TDesC &aItem) {
	switch (iTagIndex) {
	case 0:
		iFront = aItem;
		break;
	case 1:
		addVName(aItem);
		break;
	case 2:
		addVLibrary(aItem);
		break;
	case 3:
		addILibrary(aItem);
		break;
	case 4:
		addFLibrary(aItem);
	}
}

TInt XMLBuffer::getNumLibs() {
	TInt numi = 1; // frontend
	TInt nv = iVis->Count();
	// number of inputs in all visualizations
	for (TInt i = 0; i < nv; i++)
		numi += (*iVis)[i]->getNumInputs();
	return nv + numi;
}

TInt XMLBuffer::getNumVLibs() {
	return iVis->Count();
}

TInt XMLBuffer::getNumILibs(TInt aPos) {
	return (*iVis)[aPos]->getNumInputs();
}

void XMLBuffer::Parse(const TDesC& aFileName) {
	TBuf<100> coreLocation;
	_LIT(KLocation, "c:\\data\\others\\glaze\\xml\\%S");
	coreLocation.Format(KLocation, &aFileName);
	readFile(coreLocation);// read core
	TBuf<100> frontLocation;
	frontLocation.Format(KLocation, &iFront);
	readFile(frontLocation);// read frontend
	// read visualizations
	TInt num = iVisNames.Count();
	for (TInt i = 0; i < num; i++) {
		TBuf<100> visLocation;
		visLocation.Format(KLocation, &iVisNames[i]);
		readFile(visLocation);
	}
}

void XMLBuffer::readFile(const TDesC& aFileName) {
	RFs fs;
	RFile file;

	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	User::LeaveIfError(file.Open(fs, aFileName, EFileStream));
	CleanupClosePushL(file);

	// Create the XML Parser and set the default document handlers
	CSyParser* reader = CSyParser::NewLC(TPtrC());
	CDocumentHandler xmlOutput(*this);
	reader->SetHandler(&xmlOutput);
	CDeclarationHandler xmlOutput2(*this);
	reader->SetHandler(&xmlOutput2);

	// buffer size
	const int KMaxBufLen = 128;
	TBuf8<KMaxBufLen> buf;
	TBool isFinal = ETrue;
	TSyExpatStatus status = EStatusOk;
	do {
		User::LeaveIfError(file.Read(buf, KMaxBufLen));
		isFinal = (buf.Length() != KMaxBufLen);
		status = reader->Parse(buf, isFinal);
	} while (status == EStatusOk && !isFinal);
	CleanupStack::PopAndDestroy(2);
	CleanupStack::PopAndDestroy();
}
