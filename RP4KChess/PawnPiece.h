#pragma once
#include "ChessPiece.h"

class PawnPiece : public ChessPiece {
public:
	int GetFrameIndex() override { return 5; }

	void GetPossibleMoves(
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result) override;
};