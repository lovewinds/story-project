#include "drawable/EDrawable.h"

EDrawable::EDrawable()
{
}


EDrawable::~EDrawable()
{
}

void EDrawable::setBlendMode(SDL_BlendMode blending)
{
	/* Set blending function */
	SDL_SetTextureBlendMode(mTexture, blending);
}

void EDrawable::setAlpha(Uint8 alpha)
{
	/* Modulate texture alpha */
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void EDrawable::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	/* Modulate texture rgb */
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}
