#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "ChessBoard.h"

class ChessEngine {
public:
	glm::ivec4 backgroundColor = { 63, 127, 63, 255 };
	glm::ivec4 moveColor = { 255, 0, 0, 127 };

	SDL_Window* window;
	SDL_Renderer* renderer;

	ChessBoard board{ { 8, 8 } };

	bool isCellSelected = false;
	glm::ivec2 selectedPos{};

	glm::ivec2 GetWindowSize();
	void SetDrawColor(glm::ivec4 color);

	void TryMovingTo(glm::ivec2 pos);
	void TrySelecting(glm::ivec2 pos);

	void MouseClicked(glm::ivec2 pos);

	void Start();
	void End();
	bool Update();
	void Render();
};