#include "RookPiece.h"
#include "BoardWalker.h"

void RookPiece::GetPossibleMoves(
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	std::vector<glm::ivec2> dirs = {
		{ 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 },
	};

	BoardWalker::GetPossibleMoves(pos, dirs, board, result);
}