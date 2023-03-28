#pragma once
#include "ChessPiece.h"

class KnightPiece : public ChessPiece {
	int GetFrameIndex() override { return 3; }
};