#pragma once
#include "ChessPiece.h"

class KnightPiece : public ChessPiece {
	int GetFrameIndex() override { return 3; }

	void GetPossibleMoves(
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result) override;
};