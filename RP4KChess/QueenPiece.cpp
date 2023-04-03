#include "QueenPiece.h"
#include "MoveBuilder.h"

void QueenPiece::GetPossibleMoves(
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	MoveBuilder::WalkBoard(dirs, pos, board, result);
}