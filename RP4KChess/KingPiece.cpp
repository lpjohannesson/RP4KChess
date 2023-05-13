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

void KingPiece::GetMovesNotInCheck(
	glm::ivec2 kingPos,
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	std::vector<glm::ivec2> possibleMoves;
	GetPossibleMoves(pos, board, possibleMoves);

	ChessCell kingCell = board.GetCell(pos);
	board.SetCell(pos, { PieceType::None });

	for (glm::ivec2 move : possibleMoves) {
		ChessCell previousCell = board.GetCell(move);

		board.SetCell(move, kingCell);

		if (!board.InCheck(move)) {
			result.push_back(move);
		}

		board.SetCell(move, previousCell);
	}

	board.SetCell(pos, kingCell);
}
