#include "RookPiece.h"
#include "MoveBuilder.h"

void BishopPiece::GetPossibleMoves(
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	std::vector<glm::ivec2> dirs = {
		{ -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 }
	};

	MoveBuilder::WalkBoard(pos, dirs, board, result);
}