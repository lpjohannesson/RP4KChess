#pragma once
#include "ChessPiece.h"

class PawnPiece : public ChessPiece {
	int GetFrameIndex() override { return 5; }
};