#include "CSceneNodeAnimatorSineMove.h"

namespace irr {
	namespace scene {
		void CSceneNodeAnimatorSineMove::begin(ISceneNode* node) {
			startPos = node->getPosition();
			movement = targetPos - startPos;
		}

		void CSceneNodeAnimatorSineMove::animateNode(ISceneNode* node, u32 timeMs) {
			if (check(node, timeMs)) {
				core::vector3df pos = node->getPosition();

				pos += modify(movement);
				pos += sineMove * (sinf(core::PI / endTimeMs * (timeMs - startTimeMs)) - sinf(core::PI / endTimeMs * (timeMs - differenceTimeMs - startTimeMs)));

				node->setPosition(pos);
			}
		}

		void CSceneNodeAnimatorSineMove::end(ISceneNode* node) {
			node->setPosition(targetPos);
		}

		ISceneNodeAnimator* CSceneNodeAnimatorSineMove::createClone(ISceneNode* node, ISceneManager* newManager) {
			CSceneNodeAnimatorSineMove* clone = new CSceneNodeAnimatorSineMove(endTimeMs, targetPos, sineMove);
			clone->startPos = startPos;
			clone->movement = movement;
			return clone;
		}
	}
}