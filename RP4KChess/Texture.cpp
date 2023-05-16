#include "Texture.h"

SDL_Texture* Texture::GetTexture()
{
	return texture;
}

glm::ivec2 Texture::GetSize()
{
	return size;
}

void Texture::Start(SDL_Renderer* renderer, SDL_Surface* surface) {
	size = { surface->w, surface->h };

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	SDL_FreeSurface(surface);
}

void Texture::End() {
	SDL_DestroyTexture(texture);
}
