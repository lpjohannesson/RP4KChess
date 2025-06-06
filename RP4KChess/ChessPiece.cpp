#include "ChessPiece.h"
#include "ChessBoard.h"

void ChessPiece::GetMovesNotInCheck(
	glm::ivec2 kingPos,
	glm::ivec2 pos,
	ChessBoard& board,
	std::vector<glm::ivec2>& result)
{
	std::vector<glm::ivec2> possibleMoves;
	GetPossibleMoves(pos, board, possibleMoves);

	ChessCell cell = board.GetCell(pos);
	board.SetCell(pos, { PieceType::None });

	for (glm::ivec2 move : possibleMoves) {
		ChessCell previousCell = board.GetCell(move);
		board.SetCell(move, cell);

		if (!board.InCheck(kingPos)) {
			result.push_back(move);
		}

		board.SetCell(move, previousCell);
	}

	board.SetCell(pos, cell);
}
