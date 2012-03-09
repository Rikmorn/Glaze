/*
 * MeshList.h
 *
 *  Created on: 27/Mai/2009
 *      Author: Daemoniorum
 */

#ifndef MESHLIST_H_
#define MESHLIST_H_

#include "GlazeMesh.h"

class CMeshList{
public:
	static CMeshList* NewL(TInt aNumMesh);
	virtual ~CMeshList();

protected:
	CMeshList(TInt aNumMesh);
	void ConstructL(void);
	
public:
	void renderMeshes();
	void setPosition(TInt aMesh, TInt aX, TInt aY, TInt aZ);
	void setTexture(TInt aMesh, TTexture aTex);
	void setName(TInt aMesh, const TDesC &aName);
	void setAreaName(const TDesC &aName);
	void setFontTexture(TInt aMesh, TTexture aTex);
	void setAreaFontTexture(TTexture aTex);
	void setInfo(TInt aMesh, TInt aTab, const TDesC &aData);
	void setAreaInfo(TInt aTab, const TDesC &aData);
	CGlazeMesh* getMesh(TInt aPos);
	TInt meshNum();
	TBool meshMoved(TInt aMesh, TInt aUX, TInt aUY, TInt aUZ);
	void getCurrentPosition(TInt aMesh, TReal &aX, TReal &aY, TReal &aZ);
	TPtrC getMeshName(TInt aMesh);
	void MeshContent(TInt aMesh, TInt aActiveTab);
	TInt closestTarget(TReal aAngle);
	
private:
	CGlazeMesh *iAreaMesh;
	RPointerArray<CGlazeMesh>* iMeshList;
};

#endif /* MESHLIST_H_ */
