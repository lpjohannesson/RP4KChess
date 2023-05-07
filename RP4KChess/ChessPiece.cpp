#include "ChessPiece.h"
#include "ChessBoard.h"

void ChessPiece::GetMovesNotInCheck(glm::ivec2 pos, ChessBoard& board, std::vector<glm::ivec2>& result)
{
	ChessCell cell = board.GetCell(pos);

	std::vector<glm::ivec2> possibleMoves;
	GetPossibleMoves(pos, board, possibleMoves);

	board.SetCell(pos, { PieceType::None });

	glm::ivec2 kingPos = board.FindCell({ PieceType::King, cell.color });

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
