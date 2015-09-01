#pragma once
#include <string>

#include "ETexture.h"

class ETextTexture :
	public ETexture
{
public:
	ETextTexture();
	virtual ~ETextTexture();

	/* Creates image from font string */
	bool loadFromRenderedText(const std::string& textureText, SDL_Color textColor, SDL_Color bgColor);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

protected:
	/* String to show */
	std::string message;

	/* Update String */
	virtual void update_string(const std::string& str);
};
