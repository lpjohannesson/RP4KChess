#include "KingPiece.h"
#include "MoveBuilder.h"

void KingPiece::GetPossibleMoves(
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	std::vector<glm::ivec2> points;

	for (glm::ivec2 dir : dirs) {
		points.push_back(pos + dir);
	}

	MoveBuilder::TestPoints(points, pos, board, result);
}
