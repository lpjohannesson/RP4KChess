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
			engine->OnEnteredCheckmate(PieceColor::Black);
		}
		else {
			engine->OnEnteredCheck(PieceColor::Black);
		}

		return;
	}

	if (InCheck(whiteKingPos)) {
		if (InCheckmate(whiteKingPos)) {
			engine->OnEnteredCheckmate(PieceColor::White);
		}
		else {
			engine->OnEnteredCheck(PieceColor::White);
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
	glm::ivec2 cellPos = boardPos + pos * cellSize;
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

			if (cell.type == PieceType::None || cell.type == PieceType::King) {
				continue;
			}

			if (cell.color == kingCell.color) {
				continue;
			}

			ChessPiece* piece = GetPieceFromType(cell.type);

			std::vector<glm::ivec2> piecePossibleMoves;
			piece->GetPossibleMoves(cellPos, *this, piecePossibleMoves);

			if (std::find(piecePossibleMoves.begin(), piecePossibleMoves.end(), kingPos) != piecePossibleMoves.end()) {
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
			piece->GetMovesNotInCheck(cellPos, *this, possibleMoves);

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

		SetCell({ x, 0 }, { endType, PieceColor::White });
		SetCell({ x, 7 }, { endType, PieceColor::Black });

		SetCell({ x, 1 }, { PieceType::Pawn, PieceColor::White });
		SetCell({ x, 6 }, { PieceType::Pawn, PieceColor::Black });
	}
}

void ChessBoard::Start(ChessEngine* engine) {
	this->engine = engine;

	SDL_Surface* piecesSurface = IMG_Load("assets/pieces.png");
	piecesTexture.Start(engine->renderer, piecesSurface);

	piecesTextureCellSize = piecesTexture.size / glm::ivec2(6, 2);
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

	SDL_RenderCopy(renderer, piecesTexture.texture, &frameRect, rect);
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
			glm::ivec2 cellPos = { x, y };
			SDL_Rect cellRect = GetCellRect(cellPos, boardPos);

			ChessCell cell = GetCell(cellPos);

			if (cell.type == PieceType::None) {
				continue;
			}

			DrawPiece(cell, &cellRect);
		}
	}
}