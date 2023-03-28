#pragma once
#include "ChessPiece.h"

class QueenPiece : public ChessPiece {
	int GetFrameIndex() override { return 1; }

	void GetPossibleMoves(
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result) override;
};