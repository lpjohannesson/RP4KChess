#pragma once
#include "ChessPiece.h"

class BishopPiece : public ChessPiece {
	int GetFrameIndex() override { return 2; }

	void GetPossibleMoves(
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result) override;
};