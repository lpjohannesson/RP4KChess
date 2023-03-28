#include "ChessBoard.h"
#include <iostream>
#include <sstream>
#include "ChessEngine.h"
#include <SDL2/SDL_image.h>

int ChessBoard::GetCellIndex(glm::ivec2 pos) {
	return pos.y * size.x + pos.x;
}

ChessBoard::ChessBoard(glm::ivec2 size) {
	SetSize(size);
}

void ChessBoard::SetSize(glm::ivec2 size) {
	this->size = size;

	cells.clear();
	cells.reserve((std::size_t)(size.x * size.y));

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			cells.push_back({ PieceType::None });
		}
	}
}

glm::ivec2 ChessBoard::GetBoardPos() {
	glm::ivec2 boardSize = GetBoardSize();
	glm::ivec2 windowSize = engine->GetWindowSize();
	return windowSize / 2 - boardSize / 2;
}

glm::ivec2 ChessBoard::GetBoardSize() {
	return size * cellSize;
}

SDL_Rect ChessBoard::GetBoardRect(glm::ivec2 boardPos) {
	glm::ivec2 boardSize = GetBoardSize();

	return { boardPos.x, boardPos.y, boardSize.x, boardSize.y };
}

SDL_Rect ChessBoard::GetCellRect(glm::ivec2 pos, glm::ivec2 boardPos) {
	glm::ivec2 cellPos = boardPos + glm::ivec2(pos.x, pos.y) * cellSize;
	return { cellPos.x, cellPos.y, cellSize.x, cellSize.y };
}

ChessCell ChessBoard::GetCell(glm::ivec2 pos) {
	if (!CellInRange(pos)) {
		return { PieceType::None };
	}

	return cells[GetCellIndex(pos)];
}

void ChessBoard::SetCell(glm::ivec2 pos, ChessCell cell) {
	if (!CellInRange(pos)) {
		return;
	}

	cells[GetCellIndex(pos)] = cell;
}

bool ChessBoard::CellInRange(glm::ivec2 pos) {
	return
		pos.x >= 0 && pos.x < size.x&&
		pos.y >= 0 && pos.y < size.y;
}

ChessPiece* ChessBoard::GetPieceFromType(PieceType type) {
	switch (type) {
	case PieceType::None:
		return nullptr;

	case PieceType::King:
		return &king;

	case PieceType::Queen:
		return &queen;

	case PieceType::Bishop:
		return &bishop;

	case PieceType::Knight:
		return &knight;

	case PieceType::Rook:
		return &rook;

	case PieceType::Pawn:
		return &pawn;
	}
}

void ChessBoard::LoadStartingPosition() {
	constexpr PieceType endTypes[] = {
		PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
		PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook };
		
	for (int x = 0; x < 8; x++) {
		PieceType endType = endTypes[x];

		SetCell({ x, 0 }, { endType, PieceColor::White });
		SetCell({ x, 7 }, { endType, PieceColor::Black });

		SetCell({ x, 1 }, { PieceType::Pawn, PieceColor::White });
		SetCell({ x, 6 }, { PieceType::Pawn, PieceColor::Black });
	}
}

void ChessBoard::Start(ChessEngine* engine) {
	this->engine = engine;

	SDL_Surface* piecesSurface = IMG_Load("assets/pieces.png");

	if (piecesSurface == NULL) {
		std::cerr << "Could not load pieces texture." << std::endl;
	}
	else {
		piecesTextureSize = { piecesSurface->w, piecesSurface->h };
		piecesTextureCellSize = piecesTextureSize / glm::ivec2(6, 2);
	}

	piecesTexture = SDL_CreateTextureFromSurface(engine->renderer, piecesSurface);
	SDL_FreeSurface(piecesSurface);
}

void ChessBoard::End() {
	SDL_DestroyTexture(piecesTexture);
}

void ChessBoard::RenderBoard() {
	SDL_Renderer* renderer = engine->renderer;

	glm::ivec2 boardPos = GetBoardPos();
	SDL_Rect boardRect = GetBoardRect(boardPos);

	engine->SetDrawColor(boardColor);
	SDL_RenderDrawRect(renderer, &boardRect);

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			if ((x + y) % 2 != 0) {
				continue;
			}

			SDL_Rect cellRect = GetCellRect({ x, y }, boardPos);
			SDL_RenderFillRect(renderer, &cellRect);
		}
	}
}

void ChessBoard::RenderPieces() {
	SDL_Renderer* renderer = engine->renderer;
	glm::ivec2 boardPos = GetBoardPos();

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			SDL_Rect cellRect = GetCellRect({ x, y }, boardPos);

			ChessCell cell = GetCell({ x, y });
			ChessPiece* piece = GetPieceFromType(cell.type);

			if (piece == nullptr) {
				continue;
			}

			glm::ivec2 framePos = {
				piece->GetFrameIndex() * piecesTextureCellSize.x,
				(cell.color == PieceColor::Black) ? piecesTextureCellSize.y : 0
			};

			SDL_Rect frameRect = {
				framePos.x, framePos.y,
				piecesTextureCellSize.x, piecesTextureCellSize.y };

			SDL_RenderCopy(renderer, piecesTexture, &frameRect, &cellRect);
		}
	}
}