#pragma once
#include <SDL.h>

class EDrawable
{
public:
	EDrawable();
	virtual ~EDrawable();

	/* Set blending */
	void setBlendMode(SDL_BlendMode blending);

	/* Set alpha modulation */
	void setAlpha(Uint8 alpha);

	/* Set color modulation */
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

protected:
	/* The actual hardware texture */
	SDL_Texture* mTexture;
};
