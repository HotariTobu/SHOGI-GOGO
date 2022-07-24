#include "AnimatorPromotion.h"

void AnimatorPromotion::begin(ISceneNode* node) {
	startPos = node->getPosition();
	startRotation = node->getRotation();
}

void AnimatorPromotion::animateNode(ISceneNode* node, u32 timeMs) {
	if (check(node, timeMs)) {
		Piece* piece = (Piece*)node;

		vector3df pos = piece->getPosition();
		vector3df rotation = piece->getRotation();

		pos.Y = startPos.Y + sinf(PI * 345 / 360 / endTimeMs * (timeMs - startTimeMs));
		rotation.X -= modify(10.0f);
		rotation.Z += modify(180.0f);

		piece->setPosition(pos);
		piece->setRotation(rotation);
	}
}

void AnimatorPromotion::end(ISceneNode* node) {
	Piece* piece = (Piece*)node;
	piece->setPosition(startPos);
	piece->setRotation(startRotation);
	piece->setPromotion(!piece->isPromoted());
}

ISceneNodeAnimator* AnimatorPromotion::createClone(ISceneNode* node, ISceneManager* newManager) {
	return new AnimatorPromotion(endTimeMs);
}