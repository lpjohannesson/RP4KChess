#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class Texture {
public:
	SDL_Texture* texture;
	glm::ivec2 size;

	void Start(SDL_Renderer* renderer, SDL_Surface* surface);
	void End();
};