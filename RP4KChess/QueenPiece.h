#pragma once
#include "ChessPiece.h"

class QueenPiece : public ChessPiece {
private:
	std::vector<glm::ivec2> dirs = {
		{ 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 },
		{ -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 }
	};

public:
	int GetFrameIndex() override { return 1; }

	void GetPossibleMoves(
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result) override;
};