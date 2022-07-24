#pragma once

#ifndef __SCALE_DEFINER_H_INCLUDED__
#define __SCALE_DEFINER_H_INCLUDED__

#include "MyHead.h"

enum E_SCALE_NAMES {
	ESN_CAMERA_RADIUS = 0,
	ESN_PIECE_HEIGHT,
	ESN_TATAMI_HEIGHT,
	ESN_BOARD_VERTICAL,
	ESN_BOARD_HORIZON,
	ESN_BOARD_THICKNESS,
	ESN_BOARD_FOOT_HEIGHT,
	ESN_BOARD_FOOT_MARGIN,
	ESN_BOARD_POOL_HEIGHT,
	ESN_BOARD_POOL_OUT_HEIGHT,
	ESN_BOARD_POOL_OUT_WIDTH,
	ESN_BOARD_POOL_IN_HEIGHT,
	ESN_BOARD_POOL_IN_WIDTH,
	ESN_STAND_HEIGHT,
	ESN_STAND_MARGIN,
	ESN_RECTANGLE_HEIGHT,
	ESN_RECTANGLE_WIDTH,
	ESN_RECTANGLE_LINE_THICKNESS,
	ESN_COUNT
};

constexpr const c8* ScaleNameStrs[] = {
	"cameraRadius",
	"pieceHeight",
	"tatamiHeight",
	"boardVertical",
	"boardHorizon",
	"boardThickness",
	"boardFootHeight",
	"boardFootMargin",
	"boardPoolHeight",
	"boardPoolOutHeight",
	"boardPoolOutWidth",
	"boardPoolInHeight",
	"boardPoolInWidth",
	"standHeight",
	"standMargin",
	"rectangleHeight",
	"rectangleWidth",
	"rectangleLineThickness",
};

class ScaleDefiner {
public:
	ScaleDefiner(IrrlichtDevice* device);

	f32 getScale(E_SCALE_NAMES sn);

private:
	f32 Scales[ESN_COUNT];
};

#endif // !__SCALE_DEFINER_H_INCLUDED__