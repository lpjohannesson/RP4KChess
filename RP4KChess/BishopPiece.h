#pragma once
#include "ChessPiece.h"

class BishopPiece : public ChessPiece {
private:
	std::vector<glm::ivec2> dirs = {
		{ -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 }
	};

public:
	int GetFrameIndex() override { return 2; }

	void GetPossibleMoves(
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result) override;
};