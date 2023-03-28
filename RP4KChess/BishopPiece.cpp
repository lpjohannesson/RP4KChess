#include "RookPiece.h"
#include "MoveBuilder.h"

void RookPiece::GetPossibleMoves(
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	MoveBuilder::WalkBoard(pos, dirs, board, result);
}