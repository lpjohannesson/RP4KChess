#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include "ChessBoard.h"
#include "Texture.h"

class ChessEngine {
public:
	glm::ivec4 backgroundColor = { 63, 127, 63, 255 };
	glm::ivec4 moveColor = { 255, 0, 0, 127 };
	glm::ivec4 fontColor = { 0, 0, 0, 255 };

	SDL_Window* window;
	SDL_Renderer* renderer;

	TTF_Font* font;

	Texture
		blackCheckTexture,
		whiteCheckTexture,
		blackCheckmateTexture,
		whiteCheckmateTexture;
	
	Texture* currentTextTexture = nullptr;

	ChessBoard board{ { 8, 8 } };

	bool isCellSelected = false;
	glm::ivec2 selectedPos{};

	void CreateTextTexture(Texture& texture, const char* text, glm::ivec4 color);

	glm::ivec2 GetWindowSize();
	void SetDrawColor(glm::ivec4 color);

	void TrySelecting(glm::ivec2 pos);
	void TryMovingTo(glm::ivec2 pos);

	void MoveCell(glm::ivec2 from, glm::ivec2 to);

	void MouseClicked(glm::ivec2 pos);

	void EnteredCheck(PieceColor color);
	void EnteredCheckmate(PieceColor color);

	void Start();
	void End();
	bool Update(float delta);
	
	void RenderText(Texture& texture);
	void Render();
};