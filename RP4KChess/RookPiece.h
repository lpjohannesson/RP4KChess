#pragma once
#include "ChessPiece.h"

class RookPiece : public ChessPiece {
	int GetFrameIndex() override { return 4; }

	void GetPossibleMoves(
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result) override;
};