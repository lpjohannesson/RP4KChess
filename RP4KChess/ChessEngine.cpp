#include "ChessEngine.h"

glm::ivec2 ChessEngine::GetWindowSize() {
	glm::ivec2 windowSize{};
	SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);

	return windowSize;
}

void ChessEngine::CreateTextTexture(Texture& texture, const char* text, glm::ivec4 color) {
	SDL_Color sdlColor = { color.r, color.g, color.b, color.a };

	texture.Start(
		renderer, TTF_RenderText_Blended(font, text, sdlColor));
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
	glm::ivec2 kingPos = board.FindCell({ PieceType::King, cell.color });

	std::vector<glm::ivec2> possibleMoves;
	piece->GetMovesNotInCheck(kingPos, pos, board, possibleMoves);

	if (possibleMoves.empty()) {
		return;
	}

	selectedPos = pos;
	isCellSelected = true;
}

void ChessEngine::TryMovingTo(glm::ivec2 pos) {
	ChessCell cell = board.GetCell(selectedPos);

	if (cell.type == PieceType::None) {
		return;
	}

	ChessPiece* piece = board.GetPieceFromType(cell.type);
	glm::ivec2 kingPos = board.FindCell({ PieceType::King, cell.color });

	std::vector<glm::ivec2> possibleMoves;
	piece->GetMovesNotInCheck(kingPos, selectedPos, board, possibleMoves);

	if (std::find(possibleMoves.begin(), possibleMoves.end(), pos) != possibleMoves.end()) {
		MoveCell(selectedPos, pos);
	}

	isCellSelected = false;
}

void ChessEngine::MoveCell(glm::ivec2 from, glm::ivec2 to) {
	currentTextTexture = nullptr;

	board.MoveCell(from, to);
}

void ChessEngine::MouseClicked(glm::ivec2 pos) {
	glm::ivec2 cellPos = (pos - board.GetRectPos()) / board.cellSize;

	if (isCellSelected) {
		TryMovingTo(cellPos);
	}
	else {
		TrySelecting(cellPos);
	}

	Render();
}

void ChessEngine::EnteredCheck(PieceColor color) {
	currentTextTexture = color == PieceColor::Black ? &blackCheckTexture : &whiteCheckTexture;
}

void ChessEngine::EnteredCheckmate(PieceColor color) {
	currentTextTexture = color == PieceColor::Black ? &whiteCheckmateTexture : &blackCheckmateTexture;
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
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	TTF_Init();
	font = TTF_OpenFont("assets/CreatoDisplay.ttf", 24);

	CreateTextTexture(
		blackCheckTexture, "Black is in check!", fontColor);

	CreateTextTexture(
		whiteCheckTexture, "White is in check!", fontColor);

	CreateTextTexture(
		blackCheckmateTexture, "Checkmate! Black wins!", fontColor);

	CreateTextTexture(
		whiteCheckmateTexture, "Checkmate! White wins!", fontColor);

	const char* error = SDL_GetError();

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

	TTF_CloseFont(font);

	blackCheckTexture.End();
	whiteCheckTexture.End();
	blackCheckmateTexture.End();
	whiteCheckmateTexture.End();

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

void ChessEngine::RenderText(Texture& texture) {
	glm::ivec2
		rectPos = board.GetRectPos(),
		rectSize = board.GetRectSize();

	glm::ivec2 textureSize = texture.GetSize();

	SDL_Rect rect = {
		rectPos.x + (rectSize.x / 2) - (textureSize.x / 2),
		rectPos.y - textureSize.y - 8,
		textureSize.x, textureSize.y };

	SDL_RenderCopy(renderer, texture.GetTexture(), NULL, &rect);
}

void ChessEngine::Render() {
	SetDrawColor(backgroundColor);
	SDL_RenderClear(renderer);

	board.RenderBoard();

	glm::ivec2 rectPos = board.GetRectPos();
	glm::ivec2 rectSize = board.GetRectSize();

	if (isCellSelected) {
		ChessCell selectedCell = board.GetCell(selectedPos);

		if (selectedCell.type != PieceType::None) {
			ChessPiece* piece = board.GetPieceFromType(selectedCell.type);
			glm::ivec2 kingPos = board.FindCell({ PieceType::King, selectedCell.color });

			std::vector<glm::ivec2> possibleMoves;
			piece->GetMovesNotInCheck(kingPos, selectedPos, board, possibleMoves);

			SetDrawColor(moveColor);

			for (glm::ivec2 pos : possibleMoves) {
				SDL_Rect cellRect = board.GetCellRect(pos, rectPos);

				SDL_RenderFillRect(renderer, &cellRect);
			}
		}
	}

	board.RenderPieces();
	
	glm::ivec2 turnRectSize = board.cellSize * 2;

	SDL_Rect turnRect = {
		rectPos.x + rectSize.x,
		rectPos.y + rectSize.y - turnRectSize.y,
		turnRectSize.x, turnRectSize.y };
	
	board.DrawPiece({ PieceType::King, board.GetTurnColor() }, &turnRect);

	if (currentTextTexture != nullptr) {
		RenderText(*currentTextTexture);
	}

	SDL_RenderPresent(renderer);
}
