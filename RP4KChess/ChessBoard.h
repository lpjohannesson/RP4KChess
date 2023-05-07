#pragma once
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "ChessPiece.h"
#include "KingPiece.h"
#include "QueenPiece.h"
#include "BishopPiece.h"
#include "KnightPiece.h"
#include "RookPiece.h"
#include "PawnPiece.h"

#include "Texture.h"

class ChessEngine;

enum class PieceType {
	None, King, Queen, Bishop, Knight, Rook, Pawn
};

enum class PieceColor {
	Black, White
};

class ChessCell {
public:
	PieceType type;
	PieceColor color;
};

class ChessBoard {
private:
	std::vector<ChessCell> cells;
	glm::ivec2 size;

	KingPiece king;
	QueenPiece queen;
	BishopPiece bishop;
	KnightPiece knight;
	RookPiece rook;
	PawnPiece pawn;

	Texture piecesTexture;
	glm::ivec2 piecesTextureCellSize;

	ChessEngine* engine;

	PieceColor turnColor = PieceColor::White;

	int GetCellIndex(glm::ivec2 pos);

	void ScanForInCheck();

public:
	glm::ivec2 cellSize = { 48, 48 };
	glm::ivec4 boardColor = { 255, 255, 255, 255 };

	ChessBoard(glm::ivec2 size);

	glm::ivec2 GetSize();
	void SetSize(glm::ivec2 size);

	PieceColor GetTurnColor();

	glm::ivec2 GetBoardPos();
	glm::ivec2 GetBoardSize();

	SDL_Rect GetBoardRect(glm::ivec2 boardPos);
	SDL_Rect GetCellRect(glm::ivec2 pos, glm::ivec2 boardPos);

	ChessCell GetCell(glm::ivec2 pos);
	void SetCell(glm::ivec2 pos, ChessCell cell);

	glm::ivec2 FindCell(ChessCell cell);

	void MoveCell(glm::ivec2 from, glm::ivec2 to);

	bool CellInRange(glm::ivec2 pos);

	ChessPiece* GetPieceFromType(PieceType type);

	bool InCheck(glm::ivec2 kingPos);
	bool InCheckmate(glm::ivec2 kingPos);

	void LoadStartingPosition();
	
	void Start(ChessEngine* engine);
	void End();

	void DrawPiece(ChessCell cell, SDL_Rect* rect);

	void RenderBoard();
	void RenderPieces();
};