#pragma once
#include "ChessPiece.h"

class KingPiece : public ChessPiece {
	int GetFrameIndex() override { return 0; }
};