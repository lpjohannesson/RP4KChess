#include "ChessEngine.h"

glm::ivec2 ChessEngine::GetWindowSize() {
	glm::ivec2 windowSize;
	SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);

	return windowSize;
}

void ChessEngine::SetDrawColor(glm::ivec4 color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void ChessEngine::TrySelecting(glm::ivec2 pos) {
	ChessCell cell = board.GetCell(pos);

	if (cell.type == PieceType::None) {
		return;
	}

	if (cell.color != board.GetTurnColor()) {
		return;
	}

	ChessPiece* piece = board.GetPieceFromType(cell.type);

	std::vector<glm::ivec2> possibleMoves;
	piece->GetMovesNotInCheck(pos, board, possibleMoves);

	if (possibleMoves.empty()) {
		return;
	}

	selectedPos = pos;
	isCellSelected = true;
}

void ChessEngine::TryMovingTo(glm::ivec2 pos) {
	ChessCell selectedCell = board.GetCell(selectedPos);
	ChessPiece* selectedPiece = board.GetPieceFromType(selectedCell.type);

	if (selectedPiece == nullptr) {
		return;
	}

	std::vector<glm::ivec2> possibleMoves;
	selectedPiece->GetMovesNotInCheck(selectedPos, board, possibleMoves);

	if (std::find(possibleMoves.begin(), possibleMoves.end(), pos) != possibleMoves.end()) {
		MoveCell(selectedPos, pos);
	}

	isCellSelected = false;
}

void ChessEngine::MoveCell(glm::ivec2 from, glm::ivec2 to) {
	board.MoveCell(from, to);
}

void ChessEngine::MouseClicked(glm::ivec2 pos) {
	glm::ivec2 cellPos = (pos - board.GetBoardPos()) / board.cellSize;

	if (isCellSelected) {
		TryMovingTo(cellPos);
	}
	else {
		TrySelecting(cellPos);
	}

	Render();
}

void ChessEngine::Start() {
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(
		"Chess",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 480,
		NULL);
	
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	board.Start(this);
	board.LoadStartingPosition();

	Render();

	Uint32 lastTime = SDL_GetTicks();

	while (true) {
		Uint32 currentTime = SDL_GetTicks();
		Uint32 deltaMs = currentTime - lastTime;

		float delta = (float)deltaMs / 1000.0f;

		if (!Update(delta)) {
			break;
		}

		SDL_Delay(std::max((1000 / 60) - (int)(SDL_GetTicks() - currentTime), 0));
		lastTime = currentTime;
	}

	End();
}

void ChessEngine::End() {
	board.End();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

bool ChessEngine::Update(float delta) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return false;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button != SDL_BUTTON_LEFT) {
				break;
			}

			MouseClicked({ event.button.x, event.button.y });

			break;
		}
	}

	return true;
}

void ChessEngine::Render() {
	SetDrawColor(backgroundColor);
	SDL_RenderClear(renderer);

	board.RenderBoard();

	glm::ivec2 boardPos = board.GetBoardPos();
	glm::ivec2 boardSize = board.GetBoardSize();

	if (isCellSelected) {
		PieceType pieceType = board.GetCell(selectedPos).type;

		if (pieceType != PieceType::None) {
			ChessPiece* piece = board.GetPieceFromType(pieceType);

			std::vector<glm::ivec2> possibleMoves;
			piece->GetMovesNotInCheck(selectedPos, board, possibleMoves);

			SetDrawColor(moveColor);

			for (glm::ivec2 pos : possibleMoves) {
				SDL_Rect cellRect = board.GetCellRect(pos, boardPos);

				SDL_RenderFillRect(renderer, &cellRect);
			}
		}
	}

	board.RenderPieces();
	
	glm::ivec2 turnRectSize = board.cellSize * 2;

	SDL_Rect turnRect = {
		boardPos.x + boardSize.x,
		boardPos.y + boardSize.y - turnRectSize.y,
		turnRectSize.x, turnRectSize.y };
	
	board.DrawPiece({ PieceType::King, board.GetTurnColor() }, &turnRect);

	SDL_RenderPresent(renderer);
}
