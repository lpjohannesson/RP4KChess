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

	SDL_Texture* piecesTexture;
	glm::ivec2 piecesTextureSize;
	glm::ivec2 piecesTextureCellSize;

	ChessEngine* engine;

	int GetCellIndex(glm::ivec2 pos);

public:
	glm::ivec2 cellSize = { 48, 48 };
	glm::ivec4 boardColor = { 255, 255, 255, 255 };

	ChessBoard(glm::ivec2 size);

	void SetSize(glm::ivec2 size);

	glm::ivec2 GetBoardPos();
	glm::ivec2 GetBoardSize();

	SDL_Rect GetBoardRect(glm::ivec2 boardPos);
	SDL_Rect GetCellRect(glm::ivec2 pos, glm::ivec2 boardPos);

	ChessCell GetCell(glm::ivec2 pos);
	void SetCell(glm::ivec2 pos, ChessCell cell);

	void MoveCell(glm::ivec2 from, glm::ivec2 to);

	bool CellInRange(glm::ivec2 pos);

	ChessPiece* GetPieceFromType(PieceType type);

	void LoadStartingPosition();
	
	void Start(ChessEngine* engine);
	void End();

	void RenderBoard();
	void RenderPieces();
};