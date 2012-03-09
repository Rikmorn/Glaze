/*
 * GlazeAnimation.cpp
 *
 *  Created on: 1/Mai/2009
 *      Author: Daemoniorum
 */

#include "GlazeAnimation.h"

CGlazeAnimationManager* CGlazeAnimationManager::NewL() {
	CGlazeAnimationManager* self = new (ELeave) CGlazeAnimationManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	return self;
}

CGlazeAnimationManager::CGlazeAnimationManager() {
	iAnimList = new (ELeave) RPointerArray<AnimationMesh> ;
}

CGlazeAnimationManager::~CGlazeAnimationManager() {
	for (TInt i = 0; i < iAnimList->Count(); i++) {
		(*iAnimList)[i]->iDTimeList.Close();
		(*iAnimList)[i]->iAnimStartTimeList.Close();
		(*iAnimList)[i]->iActiveAnimations.Close();
	}
	iAnimList->ResetAndDestroy();
	iAnimList->Close();
	delete iAnimList;
}

void CGlazeAnimationManager::ConstructL(void) {
}

void CGlazeAnimationManager::addMesh(MAnimationObserver* aMesh) {
	AnimationMesh* newMesh = new (ELeave) AnimationMesh();
	newMesh->iAnimObserver = aMesh;
	newMesh->iTimeLimitList.Reset();
	newMesh->iActiveAnimations.Reset();
	newMesh->iAnimStartTimeList.Reset();
	newMesh->iDTimeList.Reset();
	for (TInt i = 0; i < aMesh->getNumAminations(); i++) {
		newMesh->iActiveAnimations.Append(EFalse);
		newMesh->iTimeLimitList.Append(0);
		newMesh->iAnimStartTimeList.Append(0);
		newMesh->iDTimeList.Append(0);
	}
	iAnimList->Append(newMesh);
}

// Leaves
// duration in miliseconds
void CGlazeAnimationManager::playAnimation(TInt aMesh, TInt aAnimation,
		TInt64 aDuration) {
	if ((*iAnimList)[aMesh]->iActiveAnimations[aAnimation]) {
		return;
	}
	if (aMesh > iAnimList->Count() - 1) {
		return;
	} else if (aAnimation
			> (*iAnimList)[aMesh]->iAnimObserver->getNumAminations() - 1) {
		return;
	}
	(*iAnimList)[aMesh]->iActiveAnimations[aAnimation] = ETrue; // activate animation
	// duration of animation in miliseconds
	(*iAnimList)[aMesh]->iTimeLimitList[aAnimation] = aDuration * 1000;
	(*iAnimList)[aMesh]->iDTimeList[aAnimation] = 0;
	(*iAnimList)[aMesh]->iAnimStartTimeList[aAnimation].HomeTime(); //current time
	(*iAnimList)[aMesh]->iAnimObserver->setupAnimation(aAnimation, aDuration); // setup the animation before animating
}

void CGlazeAnimationManager::updateAnimations() {
	for (TInt i = 0; i < iAnimList->Count(); i++) {
		for (TInt j = 0; j < (*iAnimList)[i]->iActiveAnimations.Count(); j++) {
			if ((*iAnimList)[i]->iActiveAnimations[j]) {
				//calculate dt for animation j
				// if dt bigger than animation time reset anim else update
				TTime currentTime;
				currentTime.HomeTime();
				TTimeIntervalMicroSeconds interval =
						currentTime.MicroSecondsFrom(
								(*iAnimList)[i]->iAnimStartTimeList[j]);
				(*iAnimList)[i]->iDTimeList[j] = interval.Int64();
				if ((*iAnimList)[i]->iDTimeList[j]
						> (*iAnimList)[i]->iTimeLimitList[j]) {
					(*iAnimList)[i]->iAnimObserver->updateAnimation(j,
							(*iAnimList)[i]->iTimeLimitList[j] / 1000); // do a last update with full duration
					resetAnimation(i, j); // reset animation
					(*iAnimList)[i]->iAnimObserver->animationComplete(j); // cleanup animation on the observer side
				} else {
					(*iAnimList)[i]->iAnimObserver->updateAnimation(j,
							(*iAnimList)[i]->iDTimeList[j] / 1000 /* miliseconds */);
				}
			}
		}
	}
}

void CGlazeAnimationManager::resetAnimation(TInt aMesh, TInt aAnimation) {
	(*iAnimList)[aMesh]->iActiveAnimations[aAnimation] = EFalse;; // activate animation
	(*iAnimList)[aMesh]->iTimeLimitList[aAnimation] = 0; // duration of animation
	(*iAnimList)[aMesh]->iAnimStartTimeList[aAnimation] = 0; //current time
	(*iAnimList)[aMesh]->iDTimeList[aAnimation] = 0; // delta time
}

TBool CGlazeAnimationManager::animationActive(TInt aMesh, TInt aAnimation) {
	return ((*iAnimList)[aMesh]->iActiveAnimations[aAnimation]) ? ETrue
			: EFalse;
}
