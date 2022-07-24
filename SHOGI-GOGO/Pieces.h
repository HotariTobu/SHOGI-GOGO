#pragma once

#ifndef __PIECES_H_INCLUDED__
#define __PIECES_H_INCLUDED__

#include "Piece.h"

enum E_PIECE_NAME {
	EPN_KING = 0,
	EPN_ROOK,
	EPN_BISHOP,
	EPN_GOLDGENERAL,
	EPN_SILVERGENERAL,
	EPN_KNIGHT,
	EPN_LANCE,
	EPN_PAWN,
};

class Pieces {
public:
	Pieces(IrrlichtDevice* device, ISceneNode* parent, ScaleDefiner* sd);

	ITexture* getPiece2d(E_PIECE_NAME pn);
	Piece* getPiece3d(E_PIECE_NAME pn);

private:
	ITexture* pieces2d[8];
	Piece* pieces3d[8];
};

#endif // !__PIECES_H_INCLUDED__