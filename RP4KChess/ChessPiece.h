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
		glm::ivec2 kingPos,
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result);

	virtual void CellMoved(glm::ivec2 from, glm::ivec2 to, ChessBoard& board) { }
};