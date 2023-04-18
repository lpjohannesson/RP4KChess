#include "MoveBuilder.h"

void MoveBuilder::TestPoints(
	std::vector<glm::ivec2>& points,
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	PieceColor color = board.GetCell(pos).color;

	for (glm::ivec2 point : points) {
		if (!board.CellInRange(point)) {
			continue;
		}

		ChessCell cell = board.GetCell(point);

		if (cell.type != PieceType::None && cell.color == color) {
			continue;
		}

		result.push_back(point);
	}
}

void MoveBuilder::WalkBoard(
	std::vector<glm::ivec2>& dirs,
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	PieceColor color = board.GetCell(pos).color;

	for (glm::ivec2 dir : dirs) {
		glm::ivec2 nextResult = pos;

		while (true) {
			nextResult += dir;

			if (!board.CellInRange(nextResult)) {
				break;
			}

			ChessCell cell = board.GetCell(nextResult);

			if (cell.type != PieceType::None) {
				if (cell.color != color) {
					result.push_back(nextResult);
				}

				break;
			}

			result.push_back(nextResult);
		}
	}
}