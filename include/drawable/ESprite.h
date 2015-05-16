#pragma once
#include <string>

#include "ETexture.h"

class ESprite :
	public ETexture
{
public:
	ESprite();
	~ESprite();

	/* Loads sprinte image at specified path */
	bool loadFromFile(std::string path);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

protected:
	SDL_Rect gSpriteClips[3];
	int8_t	sprite_index;
	int8_t	sprite_change;
};
