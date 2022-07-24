#pragma once

#ifndef __BOARD_H_INCLUDED__
#define __BOARD_H_INCLUDED__

#endif // !__BOARD_H_INCLUDED__


#include "IObject.h"

class Board : public IObject {
public:
	Board(ISceneNode* parent, ISceneManager* mgr, s32 id, ScaleDefiner* sd);
	~Board();

	void render() override;

private:
	f32 boardHeight;
	f32 rectangleHeight;
	f32 rectangleWidth;
	f32 mz, mx;
	SMaterial lineMaterial;

};