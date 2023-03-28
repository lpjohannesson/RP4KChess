#include "ChessEngine.h"

glm::ivec2 ChessEngine::GetWindowSize() {
	glm::ivec2 windowSize;
	SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);

	return windowSize;
}

void ChessEngine::SetDrawColor(glm::ivec4 color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void ChessEngine::MouseClicked(glm::ivec2 pos) {
	glm::ivec2 boardPos = (pos - board.GetBoardPos()) / board.cellSize;

	if (board.GetCell(boardPos).type == PieceType::None) {
		isCellSelected = false;
	}
	else {
		selectedCell = boardPos;
		isCellSelected = true;
	}

	Render();
}

void ChessEngine::Start() {
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(
		"Chess",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 480,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	renderer = SDL_CreateRenderer(window, 0, NULL);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	board.Start(this);
	board.LoadStartingPosition();

	board.SetCell({ 4, 4 }, { PieceType::Queen, PieceColor::White });
	board.SetCell({ 3, 4 }, { PieceType::Rook, PieceColor::Black });
	board.SetCell({ 2, 4 }, { PieceType::Bishop, PieceColor::White });

	Render();

	while (true) {
		if (!Update()) {
			break;
		}
	}

	End();
}

void ChessEngine::End() {
	board.End();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

bool ChessEngine::Update() {
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

	if (isCellSelected) {
		ChessPiece* piece = board.GetPieceFromType(board.GetCell(selectedCell).type);

		if (piece != nullptr) {
			std::vector<glm::ivec2> possibleMoves;
			piece->GetPossibleMoves(selectedCell, board, possibleMoves);

			SetDrawColor(moveColor);

			for (glm::ivec2 pos : possibleMoves) {
				SDL_Rect cellRect = board.GetCellRect(pos, board.GetBoardPos());

				SDL_RenderFillRect(renderer, &cellRect);
			}
		}
	}

	board.RenderPieces();

	SDL_RenderPresent(renderer);
}
