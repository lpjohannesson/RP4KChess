#include "ChessBoard.h"
#include <sstream>
#include "ChessEngine.h"
#include <SDL2/SDL_image.h>

int ChessBoard::GetCellIndex(glm::ivec2 pos) {
	return pos.y * size.x + pos.x;
}

void ChessBoard::ScanForInCheck() {
	glm::ivec2
		blackKingPos = FindCell({ PieceType::King, PieceColor::Black }),
		whiteKingPos = FindCell({ PieceType::King, PieceColor::White });

	if (InCheck(blackKingPos)) {
		if (InCheckmate(blackKingPos)) {
			engine->EnteredCheckmate(PieceColor::Black);
		}
		else {
			engine->EnteredCheck(PieceColor::Black);
		}

		return;
	}

	if (InCheck(whiteKingPos)) {
		if (InCheckmate(whiteKingPos)) {
			engine->EnteredCheckmate(PieceColor::White);
		}
		else {
			engine->EnteredCheck(PieceColor::White);
		}

		return;
	}
}

ChessBoard::ChessBoard(glm::ivec2 size) {
	SetSize(size);
}

glm::ivec2 ChessBoard::GetSize() {
	return size;
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

PieceColor ChessBoard::GetTurnColor()
{
	return turnColor;
}

glm::ivec2 ChessBoard::GetRectPos() {
	glm::ivec2 rectSize = GetRectSize();
	glm::ivec2 windowSize = engine->GetWindowSize();
	return windowSize / 2 - rectSize / 2;
}

glm::ivec2 ChessBoard::GetRectSize() {
	return size * cellSize;
}

SDL_Rect ChessBoard::GetRect(glm::ivec2 rectPos) {
	glm::ivec2 rectSize = GetRectSize();

	return { rectPos.x, rectPos.y, rectSize.x, rectSize.y };
}

SDL_Rect ChessBoard::GetCellRect(glm::ivec2 pos, glm::ivec2 rectPos) {
	glm::ivec2 cellPos = rectPos + pos * cellSize;
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

glm::ivec2 ChessBoard::FindCell(ChessCell cell)
{
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			glm::ivec2 pos = { x, y };
			ChessCell foundCell = GetCell(pos);

			if (foundCell.type == cell.type && foundCell.color == cell.color) {
				return pos;
			}
		}
	}

	return { -1, -1 };
}

void ChessBoard::MoveCell(glm::ivec2 from, glm::ivec2 to) {
	ChessCell cell = GetCell(from);
	
	SetCell(from, { PieceType::None });
	SetCell(to, cell);

	ChessPiece* piece = GetPieceFromType(cell.type);

	if (piece != nullptr) {
		piece->CellMoved(from, to, *this);
	}

	if (turnColor == PieceColor::Black) {
		turnColor = PieceColor::White;
	}
	else {
		turnColor = PieceColor::Black;
	}

	ScanForInCheck();
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

	default:
		return nullptr;
	}
}

bool ChessBoard::InCheck(glm::ivec2 kingPos)
{
	ChessCell kingCell = GetCell(kingPos);

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			glm::ivec2 cellPos = { x, y };
			ChessCell cell = GetCell(cellPos);

			if (cell.type == PieceType::None) {
				continue;
			}

			if (cell.color == kingCell.color) {
				continue;
			}

			ChessPiece* piece = GetPieceFromType(cell.type);

			std::vector<glm::ivec2> possibleMoves;
			piece->GetPossibleMoves(cellPos, *this, possibleMoves);

			if (std::find(possibleMoves.begin(), possibleMoves.end(), kingPos) != possibleMoves.end()) {
				return true;
			}
		}
	}

	return false;
}

bool ChessBoard::InCheckmate(glm::ivec2 kingPos) {
	ChessCell kingCell = GetCell(kingPos);

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			glm::ivec2 cellPos = { x, y };
			ChessCell cell = GetCell(cellPos);

			if (cell.type == PieceType::None) {
				continue;
			}

			if (cell.color != kingCell.color) {
				continue;
			}

			ChessPiece* piece = GetPieceFromType(cell.type);

			std::vector<glm::ivec2> possibleMoves;
			piece->GetMovesNotInCheck(kingPos, cellPos, *this, possibleMoves);

			if (!possibleMoves.empty()) {
				return false;
			}
		}
	}

	return true;
}

void ChessBoard::LoadStartingPosition() {
	constexpr PieceType endTypes[] = {
		PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
		PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook
	};
		
	for (int x = 0; x < 8; x++) {
		PieceType endType = endTypes[x];

		SetCell({ x, 0 }, { endType, PieceColor::Black });
		SetCell({ x, 7 }, { endType, PieceColor::White });

		SetCell({ x, 1 }, { PieceType::Pawn, PieceColor::Black });
		SetCell({ x, 6 }, { PieceType::Pawn, PieceColor::White });
	}
}

void ChessBoard::Start(ChessEngine* engine) {
	this->engine = engine;

	SDL_Surface* piecesSurface = IMG_Load("assets/pieces.png");
	piecesTexture.Start(engine->renderer, piecesSurface);

	glm::ivec2 piecesTextureSize = piecesTexture.GetSize();
	piecesTextureCellSize = piecesTextureSize / glm::ivec2(6, 2);
}

void ChessBoard::End() {
	piecesTexture.End();
}

void ChessBoard::DrawPiece(ChessCell cell, SDL_Rect* rect) {
	if (cell.type == PieceType::None) {
		return;
	}

	SDL_Renderer* renderer = engine->renderer;
	ChessPiece* piece = GetPieceFromType(cell.type);

	glm::ivec2 framePos = {
		piece->GetFrameIndex() * piecesTextureCellSize.x,
		(cell.color == PieceColor::Black) ? piecesTextureCellSize.y : 0
	};

	SDL_Rect frameRect = {
		framePos.x, framePos.y,
		piecesTextureCellSize.x, piecesTextureCellSize.y };

	SDL_RenderCopy(renderer, piecesTexture.GetTexture(), &frameRect, rect);
}

void ChessBoard::RenderBoard() {
	SDL_Renderer* renderer = engine->renderer;

	glm::ivec2 rectPos = GetRectPos();
	SDL_Rect rect = GetRect(rectPos);

	engine->SetDrawColor(boardColor);

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			if ((x + y) % 2 != 0) {
				continue;
			}

			SDL_Rect cellRect = GetCellRect({ x, y }, rectPos);
			SDL_RenderFillRect(renderer, &cellRect);
		}
	}

	SDL_RenderDrawRect(renderer, &rect);
}

void ChessBoard::RenderPieces() {
	SDL_Renderer* renderer = engine->renderer;
	glm::ivec2 rectPos = GetRectPos();

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			glm::ivec2 cellPos = { x, y };
			SDL_Rect cellRect = GetCellRect(cellPos, rectPos);

			ChessCell cell = GetCell(cellPos);

			if (cell.type == PieceType::None) {
				continue;
			}

			DrawPiece(cell, &cellRect);
		}
	}
}