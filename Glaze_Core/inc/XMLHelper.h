/*
 * XMLHelper.h
 *
 *  Created on: 30/Mar/2009
 *      Author: Daemoniorum
 */

#ifndef XMLHELPER_H_
#define XMLHELPER_H_

#include "SyExpat.h"
#include "XMLBuffer.h"

class CDocumentHandler : public CBase, public MSyDocHandler
{
public:
	CDocumentHandler(XMLBuffer& buf);
	virtual ~CDocumentHandler();
	
	virtual void StartElement(const TQualified& aName, const RArray<TAttribute>& attributes);
	virtual void EndElement(const TQualified& aName);
	virtual void CharacterData(const TDesC& aData);
	virtual void ProcessingInstruction(const TDesC& aTarget, const TDesC& aData);
	virtual void Comment(const TDesC& aData);
	virtual void StartCDataSection();
	virtual void EndCDataSection();
	virtual void Default(const TDesC& /*aData*/);
	virtual void SetParser(CExpatParserBase* aParser);

private:
	TBool captureChar;
	XMLBuffer& buffer; 
};

class CDeclarationHandler : public CBase, public MSyDeclHandler
{
public:
	CDeclarationHandler(XMLBuffer& buf);
	CDeclarationHandler();
	virtual ~CDeclarationHandler();
	
	virtual TSyExpatStatus NotStandalone();
	virtual void StartDocType( const TDesC& aDocType, const TDesC& aSystemId, const TDesC& aPublicId, const TBool aHasInternalSubset);
	
	virtual void EndDocType();
	virtual void StartNamespace(const TDesC& aPrefix, const TDesC& aURI);
	virtual void EndNamespace(const TDesC& aPrefix);
	virtual void Entity(const TDesC& aName, const TBool aIsParameter, const TDesC& aValue, const TDesC& aBase, const TDesC& aSystemId, const TDesC& aPublicId, const TDesC& aNotation);
	virtual void SkippedEntity(const TDesC& aEntity, const TBool aIsParameter);
	virtual void Notation(const TDesC& aNotationName, const TDesC& aBase, const TDesC& aSystemId, const TDesC& aPublicId);
	virtual void SetParser(CExpatParserBase* aParser);

	XMLBuffer& buffer;

};
#endif /* XMLHELPER_H_ */
