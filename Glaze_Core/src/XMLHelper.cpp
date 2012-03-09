/*
 * XMLHelper.cpp
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#include "XMLHelper.h"

CDocumentHandler::CDocumentHandler(XMLBuffer& buf) :
	buffer(buf) {
	captureChar = EFalse;
}

CDocumentHandler::~CDocumentHandler() {
}

void CDocumentHandler::StartElement(const TQualified& aName, const RArray<
		TAttribute>& attributes) {
	TPtrC elemName(aName.iLocalName);
	TInt ct = buffer.isTag(elemName);
	if (ct >= 0) {
		captureChar = ETrue;
		buffer.currentTag(ct);
	}
}

void CDocumentHandler::EndElement(const TQualified& /*aName*/) {
	captureChar = EFalse;
}

void CDocumentHandler::CharacterData(const TDesC& aData) {
	if (captureChar)
		buffer.addItem(aData);
}

void CDocumentHandler::ProcessingInstruction(const TDesC& aTarget,
		const TDesC& aData) {
	//_LIT(KElementFormatString,"PI %S = %S\n");
	//TPtrC target(aTarget);
	//TPtrC data(aData);
	//Indent(iDepth);iTest.Printf(KElementFormatString, &target, &data);
}

void CDocumentHandler::Comment(const TDesC& aData) {
	//_LIT(KElementFormatString,"Comment - %S\n");

	//TPtrC tmp(aData);
	//Indent(iDepth); iTest.Printf(KElementFormatString, &tmp);
}

void CDocumentHandler::StartCDataSection() {
	//_LIT(KDefaultFormatString,"CDATA Start\n");
	//Indent(iDepth); iTest.Printf(KDefaultFormatString);
}

void CDocumentHandler::EndCDataSection() {
	//_LIT(KDefaultFormatString,"CDATA End\n");
	//Indent(iDepth); iTest.Printf(KDefaultFormatString);
}

void CDocumentHandler::Default(const TDesC& aData) {
	//_LIT(KDefaultFormatString,"Default - S\n");
	//TPtrC tmp(aData);
	//Indent(iDepth); iTest.Printf(KDefaultFormatString, &tmp);
}

void CDocumentHandler::SetParser(CExpatParserBase* /*aParser*/) {
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
CDeclarationHandler::CDeclarationHandler(XMLBuffer& buf) :
	buffer(buf) {
}

CDeclarationHandler::~CDeclarationHandler() {
}

TSyExpatStatus CDeclarationHandler::NotStandalone() {
	return EStatusOk;
}

void CDeclarationHandler::StartDocType(const TDesC& aDocType,
		const TDesC& /*aSystemId*/, const TDesC& /*aPublicId*/, const TBool /*aHasInternalSubset*/) {
}

void CDeclarationHandler::EndDocType() {
}

void CDeclarationHandler::StartNamespace(const TDesC& aPrefix,
		const TDesC& aURI) {
}

void CDeclarationHandler::EndNamespace(const TDesC& /*aPrefix*/) {
}

void CDeclarationHandler::Entity(const TDesC& aName,
		const TBool /*aIsParameter*/, const TDesC& aValue,
		const TDesC& /*aBase*/, const TDesC& /*aSystemId*/,
		const TDesC& /*aPublicId*/, const TDesC& /*aNotation*/) {
}

void CDeclarationHandler::SkippedEntity(const TDesC& aEntity, const TBool /*aIsParameter*/) {
}

void CDeclarationHandler::Notation(const TDesC& aNotationName,
		const TDesC& /*aBase*/, const TDesC& /*aSystemId*/, const TDesC& /*aPublicId*/) {
}

void CDeclarationHandler::SetParser(CExpatParserBase* /*aParser*/) {
}
