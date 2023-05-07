#include "PawnPiece.h"
#include "MoveBuilder.h"

void PawnPiece::GetPossibleMoves(glm::ivec2 pos, ChessBoard& board, std::vector<glm::ivec2>& result) {
	PieceColor color = board.GetCell(pos).color;
	int movementSign = (color == PieceColor::Black) ? 1 : -1;

	std::vector<glm::ivec2> capturePoints = {
		pos + glm::ivec2(-1, movementSign),
		pos + glm::ivec2(1, movementSign)
	};

	for (glm::ivec2 capturePoint : capturePoints) {
		ChessCell cell = board.GetCell(capturePoint);

		if (cell.type == PieceType::None) {
			continue;
		}

		if (cell.color == color) {
			continue;
		}

		result.push_back(capturePoint);
	}

	glm::ivec2 jumpOne = pos + glm::ivec2(0, movementSign);

	if (!board.CellInRange(jumpOne)) {
		return;
	}

	if (board.GetCell(jumpOne).type != PieceType::None) {
		return;
	}

	result.push_back(jumpOne);

	bool canJumpTwo;

	if (color == PieceColor::Black) {
		canJumpTwo = (pos.y <= 1);
	}
	else {
		canJumpTwo = (pos.y >= board.GetSize().y - 2);
	}

	if (!canJumpTwo) {
		return;
	}

	glm::ivec2 jumpTwo = pos + glm::ivec2(0, movementSign * 2);

	if (!board.CellInRange(jumpTwo)) {
		return;
	}

	if (board.GetCell(jumpTwo).type != PieceType::None) {
		return;
	}

	result.push_back(jumpTwo);
}

void PawnPiece::CellMoved(glm::ivec2 from, glm::ivec2 to, ChessBoard& board) {
	ChessCell cell = board.GetCell(to);

	bool becomesQueen;

	if (cell.color == PieceColor::Black) {
		becomesQueen = (to.y >= board.GetSize().y - 1);
	}
	else {
		becomesQueen = (to.y <= 0);
	}

	if (becomesQueen) {
		board.SetCell(to, { PieceType::Queen, cell.color });
	}
}
