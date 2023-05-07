#pragma once
#include <vector>
#include <glm/glm.hpp>

class ChessBoard;

class ChessPiece {
public:
	virtual int GetFrameIndex() { return 0; }

	virtual void GetPossibleMoves(
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result) { }

	virtual void GetMovesNotInCheck(
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result);
};