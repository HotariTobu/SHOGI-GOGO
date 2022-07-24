#pragma once

#ifndef __PIECE_H_INCLUDED__
#define __PIECE_H_INCLUDED__

#include "IObject.h"

constexpr f32 sin18 = 0.30901699437495f;
constexpr f32 cos18 = 0.95105651629515f;
constexpr f32 sin81 = 0.98768834059514f;
constexpr f32 cos81 = 0.15643446504023f;
constexpr f32 cos108 = -0.30901699437495f;
constexpr f32 cos63 = 0.45399049973955f;
constexpr f32 sin85 = 0.99619469809175f;
constexpr f32 cos85 = 0.087155742747658f;
//sqrtf(2 / (1 - cos18))
constexpr f32 proE = 6.392453221499428f;
//cos108 * cos108 - 1
constexpr f32 cos2p108s1 = cos108 * cos108 - 1;

class Piece : public IObject {
public:
	Piece(ISceneNode* parent, ISceneManager* mgr, s32 id, ScaleDefiner* sd, f32 rate);
	~Piece();

	Piece* clone(ISceneNode* newParent = 0, ISceneManager * newManager = 0) override;

	f32 getRadius() const;
	void setPromotion(bool promotion);
	bool isPromoted() const;

	void temp();

private:
	Piece(ISceneNode* parent, ISceneManager* mgr, s32 id, ScaleDefiner* sd) : IObject(parent, mgr, id, sd) {}

	vector3df Vectors[10];
	vector2df Coordinates[16];

	f32 rate = 0;

	//c
	f32 radius = 0;
	bool promotion = false;

};

#endif // !__PIECE_H_INCLUDED__