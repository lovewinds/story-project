#pragma once

#include <string>

#include <SDL.h>

#include "EDrawable.hpp"

class ETextTexture : public EDrawable
{
public:
	ETextTexture(std::string text, SDL_Color textColor, SDL_Color bgColor);
	virtual ~ETextTexture();

	/* Update String */
	virtual void setText(const std::string& text);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render(int delta_x, int delta_y, double delta_angle);

protected:
	void _createTexture();
	void _removeTexture();

	/* String to show */
	std::string message;

	SDL_Color textColor;
	SDL_Color bgColor;
};
