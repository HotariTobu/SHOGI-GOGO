#pragma once

#ifndef __ANIMATORPROMOTION_H_INCLUDED__
#define __ANIMATORPROMOTION_H_INCLUDED__

#include "MyHead.h"
#include "Piece.h"
#include "ISceneNodeAnimatorEnd.h"

class AnimatorPromotion : ISceneNodeAnimatorEnd {
public:
	AnimatorPromotion(u32 endTimeMs) : ISceneNodeAnimatorEnd(endTimeMs) {}
	virtual void begin(ISceneNode* node) override;
	virtual void animateNode(ISceneNode* node, u32 timeMs) override;
	virtual void end(ISceneNode* node) override;
	virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0) override;

private:
	vector3df startPos, startRotation;
};

#endif // !__ANIMATORPROMOTION_H_INCLUDED__