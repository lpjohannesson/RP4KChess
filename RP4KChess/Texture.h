#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class Texture {
private:
	SDL_Texture* texture;
	glm::ivec2 size;

public:
	SDL_Texture* GetTexture();
	glm::ivec2 GetSize();

	void Start(SDL_Renderer* renderer, SDL_Surface* surface);
	void End();
};