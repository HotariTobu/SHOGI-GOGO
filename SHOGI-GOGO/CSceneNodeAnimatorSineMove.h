#pragma once

#ifndef __C_SCENE_NODE_ANIMATOR_SINE_MOVE_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_SINE_MOVE_H_INCLUDED__

#include "ISceneNodeAnimatorEnd.h"

namespace irr {
	namespace scene {
		class CSceneNodeAnimatorSineMove : public ISceneNodeAnimatorEnd {
		public:
			CSceneNodeAnimatorSineMove(u32 endTimeMs, core::vector3df targetPos, core::vector3df sineMove) : ISceneNodeAnimatorEnd(endTimeMs), targetPos(targetPos), sineMove(sineMove){}
			virtual void begin(ISceneNode* node) override;
			virtual void animateNode(ISceneNode* node, u32 timeMs) override;
			virtual void end(ISceneNode* node) override;
			virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0) override;

		private:
			core::vector3df startPos, movement, targetPos, sineMove;
		};
	}
}

#endif // !__C_SCENE_NODE_ANIMATOR_SINE_MOVE_H_INCLUDED__
