#include "KnightPiece.h"
#include "MoveBuilder.h"

void KnightPiece::GetPossibleMoves(
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	std::vector<glm::ivec2> points;

	glm::ivec2 normal{ 1, 0 };

	for (int i = 0; i < 4; i++) {
		glm::ivec2 basePos = pos + normal * 2;
		glm::ivec2 tangent = glm::ivec2(-normal.y, normal.x);

		points.push_back(basePos + tangent);
		points.push_back(basePos - tangent);

		normal = tangent;
	}

	MoveBuilder::TestPoints(points, pos, board, result);
}
