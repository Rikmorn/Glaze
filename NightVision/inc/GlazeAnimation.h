/*
 * GlazeAnimation.h
 *
 *  Created on: 1/Mai/2009
 *      Author: Daemoniorum
 */

#ifndef GLAZEANIMATION_H_
#define GLAZEANIMATION_H_

#include <e32std.h>
#include <e32base.h>

class MAnimationObserver {
public:
	virtual void updateAnimation(TInt aAnim, TInt64 aDTime) = 0;
	virtual TInt getNumAminations() = 0;
};

class CGlazeAnimationManager {
public:
	static CGlazeAnimationManager* NewL();
	virtual ~CGlazeAnimationManager();

protected:
	CGlazeAnimationManager();
	void ConstructL(void);
	
public:
	void playAnimation(TInt aMesh, TInt aAnimation, TInt64 aDuration);
	void updateAnimations();
	void addMesh(MAnimationObserver* aMesh);
	TBool animationActive(TInt aMesh, TInt aAnimation);

private:
	void resetAnimation(TInt aMesh, TInt aAnimation);
	
private:
	struct AnimationMesh{
		MAnimationObserver* iAnimObserver;
		RArray<TInt64> iTimeLimitList;
		RArray<TInt64> iDTimeList;
		RArray<TTime> iAnimStartTimeList;
		RArray<TBool> iActiveAnimations;
	};
	RPointerArray<AnimationMesh>* iAnimList;
};

#endif /* GLAZEANIMATION_H_ */
