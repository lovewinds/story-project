#pragma once
#include <string>
#include <memory>

#include "ETexture.h"

class EImageResourceInfo;

class ESprite : public ETexture
{
public:
	ESprite();
	~ESprite();

	/* Loads sprite image at specified path */
	bool loadFromFile(std::string path);
	bool alloc(std::string path);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

protected:
	SDL_Rect gSpriteClips[4];
	int8_t	sprite_index;
	int8_t	sprite_change;

	std::shared_ptr<EImageResourceInfo>	resource;
};
