#include "CSceneNodeAnimatorFlyCircleRotation.h"

namespace irr {
	namespace scene {
		void CSceneNodeAnimatorFlyCircleRotation::animateNode(ISceneNode* node, u32 timeMs) {
			if (this->timeMs == 0) {
				return;
			}

			core::vector3df pos(node->getPosition()), rotation(node->getRotation());

			if (isFirst) {
				isFirst = false;

				lastTimeMs = timeMs;

				f32 degree = (f32)startTimeMs / (f32)timeMs * 360.0f;

				switch (axis) {
				case X:
					pos.Y = centerPos.Y + radius;
					pos.Z = centerPos.Z;
					pos.rotateYZBy((f64)degree, centerPos);
					rotation.X += degree;
					break;

				case Y:
					pos.X = centerPos.X + radius;
					pos.Z = centerPos.Z;
					pos.rotateXZBy((f64)degree, centerPos);
					rotation.Y -= degree;
					break;

				case Z:
					pos.X = centerPos.X + radius;
					pos.Y = centerPos.Y;
					pos.rotateXYBy((f64)degree, centerPos);
					rotation.Z += degree;
					break;
				}
			}
			else {
				u32 time = timeMs - lastTimeMs;
				lastTimeMs = timeMs;

				f32 degree = (f32)time / (f32)this->timeMs * 360.0f;

				switch (axis) {
				case X:
					pos.rotateYZBy((f64)degree, centerPos);
					pos.X = node->getPosition().X;
					rotation.X += degree;
					break;

				case Y:
					pos.rotateXZBy((f64)degree, centerPos);
					pos.Y = node->getPosition().Y;
					rotation.Y -= degree;
					break;

				case Z:
					pos.rotateXYBy((f64)degree, centerPos);
					pos.Z = node->getPosition().Z;
					rotation.Z += degree;
					break;
				}
			}

			node->setPosition(pos);
			node->setRotation(rotation);
		}

		ISceneNodeAnimator* CSceneNodeAnimatorFlyCircleRotation::createClone(ISceneNode* node, ISceneManager* newManager) {
			CSceneNodeAnimatorFlyCircleRotation* clone = new CSceneNodeAnimatorFlyCircleRotation(axis, centerPos, radius, timeMs, startTimeMs);
			clone->isFirst = isFirst;
			clone->lastTimeMs = lastTimeMs;
			return clone;
		}
	}
}