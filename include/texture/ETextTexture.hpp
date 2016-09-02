#pragma once

#include <string>

#include <SDL.h>

#include "ETexture.hpp"

class ETextTexture : public ETexture
{
public:
	ETextTexture(std::string text, SDL_Color textColor, SDL_Color bgColor);
	virtual ~ETextTexture();

	/* Update String */
	virtual void setText(const std::string& text);

	bool allocate();
	void deallocate();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

protected:
	/* String to show */
	std::string message;

	SDL_Color textColor;
	SDL_Color bgColor;
};
