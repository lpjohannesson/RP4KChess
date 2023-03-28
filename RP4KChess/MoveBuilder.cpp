#include "MoveBuilder.h"
#include <queue>

void MoveBuilder::TestPoints(
	glm::ivec2 pos,
	std::vector<glm::ivec2>& points,
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
	glm::ivec2 pos,
	std::vector<glm::ivec2>& dirs,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	PieceColor color = board.GetCell(pos).color;

	std::queue<BoardWalker> walkers;

	for (glm::ivec2 dir : dirs) {
		walkers.push({ pos, dir });
	}

	while (!walkers.empty()) {
		BoardWalker& walker = walkers.front();

		walker.pos += walker.dir;

		if (!board.CellInRange(walker.pos)) {
			walkers.pop();
			continue;
		}

		ChessCell cell = board.GetCell(walker.pos);

		if (cell.type != PieceType::None) {
			if (cell.color != color) {
				result.push_back(walker.pos);
			}

			walkers.pop();
			continue;
		}

		result.push_back(walker.pos);
	}
}