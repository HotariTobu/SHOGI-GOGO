#pragma once

#ifndef __C_SCENE_NODE_ANIMATOR_FLY_CIRCLE_ROTATION_H__
#define __C_SCENE_NODE_ANIMATOR_FLY_CIRCLE_ROTATION_H__

#include <ISceneNode.h>

namespace irr {
	namespace scene {
		enum Axis
		{
			X = 0,
			Y,
			Z,
		};

		class CSceneNodeAnimatorFlyCircleRotation : public ISceneNodeAnimator {
		public:
			CSceneNodeAnimatorFlyCircleRotation(Axis axis, core::vector3df centerPos, f32 radius, s32 timeMs, s32 startTimeMs = 0) : axis(axis), centerPos(centerPos), radius(radius), timeMs(timeMs), startTimeMs(startTimeMs), isFirst(true), lastTimeMs(0) {}
			virtual void animateNode(ISceneNode* node, u32 timeMs) override;
			virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0) override;

		private:
			Axis axis;
			core::vector3df centerPos;
			f32 radius;
			s32 timeMs, startTimeMs;

			bool isFirst;
			u32 lastTimeMs;
		};
	}
}

#endif // !__C_SCENE_NODE_ANIMATOR_FLY_CIRCLE_ROTATION_H__