#include "BoardWalker.h"
#include <queue>

void BoardWalker::GetPossibleMoves(
	glm::ivec2 pos,
	std::vector<glm::ivec2>& dirs,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	PieceColor color = board.GetCell(pos).color;

	std::queue<BoardWalkerCell> walkers;

	for (glm::ivec2 dir : dirs) {
		walkers.push({ pos, dir });
	}

	while (!walkers.empty()) {
		BoardWalkerCell& walker = walkers.front();

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