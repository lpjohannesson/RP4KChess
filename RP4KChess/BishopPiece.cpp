#include "BishopPiece.h"
#include "MoveBuilder.h"

void BishopPiece::GetPossibleMoves(
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	MoveBuilder::WalkBoard(dirs, pos, board, result);
}