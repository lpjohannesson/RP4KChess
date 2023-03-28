#include "RookPiece.h"
#include "BoardWalker.h"

void BishopPiece::GetPossibleMoves(
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	std::vector<glm::ivec2> dirs = {
		{ -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 }
	};

	BoardWalker::GetPossibleMoves(pos, dirs, board, result);
}