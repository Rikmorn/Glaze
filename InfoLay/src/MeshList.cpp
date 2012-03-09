/*
 * MeshList.cpp
 *
 *  Created on: 27/Mai/2009
 *      Author: Daemoniorum
 */

#include <MeshList.h>

CMeshList* CMeshList::NewL(TInt aNumMesh) {
	CMeshList* self = new (ELeave) CMeshList(aNumMesh);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

CMeshList::CMeshList(TInt aNumMesh) {
	iMeshList = new (ELeave) RPointerArray<CGlazeMesh> ;
	iAreaMesh = CGlazeMesh::NewL();
	for (TInt i = 0; i < aNumMesh; i++) {
		CGlazeMesh* iMesh = CGlazeMesh::NewL();
		iMeshList->AppendL(iMesh);
	}
}

CMeshList::~CMeshList() {
	iMeshList->ResetAndDestroy();
	iMeshList->Close();
	delete iMeshList;
}

void CMeshList::ConstructL(void) {
}

void CMeshList::renderMeshes() {
	for (TInt i = 0; i < iMeshList->Count(); i++) {
		(*iMeshList)[i]->renderMesh();
	}
}

void CMeshList::setPosition(TInt aMesh, TInt aX, TInt aY, TInt aZ) {
	(*iMeshList)[aMesh]->setWorldPosition(aX, aY, aZ);
}

void CMeshList::setTexture(TInt aMesh, TTexture aTex) {
	(*iMeshList)[aMesh]->setTexture(aTex);
}

void CMeshList::setFontTexture(TInt aMesh, TTexture aTex) {
	(*iMeshList)[aMesh]->setFontTexture(aTex);
}

void CMeshList::setAreaFontTexture(TTexture aTex){
	iAreaMesh->setFontTexture(aTex);
}

void CMeshList::setInfo(TInt aMesh, TInt aTab, const TDesC &aData) {
	(*iMeshList)[aMesh]->addInfo(aTab, aData);
}

void CMeshList::setAreaInfo(TInt aTab, const TDesC &aData){
	iAreaMesh->addInfo(aTab, aData);
}

CGlazeMesh* CMeshList::getMesh(TInt aPos) {
	return (*iMeshList)[aPos];
}

TBool CMeshList::meshMoved(TInt aMesh, TInt aUX, TInt aUY, TInt aUZ) {
	return (*iMeshList)[aMesh]->hasMoved(aUX, aUY, aUZ);
}

TInt CMeshList::meshNum() {
	return iMeshList->Count();
}

void CMeshList::getCurrentPosition(TInt aMesh, TReal &aX, TReal &aY, TReal &aZ) {
	(*iMeshList)[aMesh]->getCurrentPos(aX, aY, aZ);
}

void CMeshList::MeshContent(TInt aMesh, TInt aActiveTab) {
	if (aMesh>=0){
		(*iMeshList)[aMesh]->renderContent(aActiveTab);
	} else {
		iAreaMesh->renderContent(aActiveTab);
	}
}

void CMeshList::setName(TInt aMesh, const TDesC &aName){
	(*iMeshList)[aMesh]->addName(aName);
}

void CMeshList::setAreaName(const TDesC &aName){
	iAreaMesh->addName(aName);
}

TPtrC CMeshList::getMeshName(TInt aMesh) {
	return (aMesh >= 0) ? (*iMeshList)[aMesh]->getName() : iAreaMesh->getName();
}

TInt CMeshList::closestTarget(TReal aAngle) {
	TInt numTargets = iMeshList->Count();
	for (TInt i = 0; i < numTargets; i++) {
		TReal mX, mY, mZ;
		(*iMeshList)[i]->getCurrentPos(mX, mY, mZ);
		TReal length;
		TReal angle;
		TReal x2;
		TReal y2;
		TReal z2;
		Math::Pow(x2, mX, 2);
		Math::Pow(y2, mY, 2);
		Math::Pow(z2, mZ, 2);
		Math::Sqrt(length, x2 + y2 + z2);
		Math::ACos(angle, (mX / length));
		angle *= 57.29577951;		
		TReal off = 10;
		TReal off1, off2;
		if (mZ >= 0) { // 1 quad looks like it works, should work with 2
			off1 = angle - off;
			if (off1 < 0) {
				TReal temp = off - angle;
				off1 = 360.0 - temp;
			}
		} else { // calculate angle for 3 and 4 quad
			// off2 a bit off
			angle = 360.0 - angle;
			off1 = angle - off;
		}
		off2 = angle + off;
		if (aAngle >= off1 && aAngle <= off2) {
			return i;
		}
		if (off1 > off2) {
			if (aAngle >= off1 || aAngle <= off2) {
				return i;
			}
		}
	}
	return -1;
}
