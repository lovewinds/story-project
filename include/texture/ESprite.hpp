#pragma once

#include <string>
#include <memory>
#include <vector>

#include "ETexture.h"

class EImageResourceInfo;

class ESprite : public ETexture
{
public:
	ESprite(std::string name, std::string base_image, std::vector<SDL_Rect>& cells);
	~ESprite();

	std::string getName();

	bool allocate();
	void deallocate();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

protected:
	ESprite();

	std::string name;
	std::string base_image_name;

	std::vector<SDL_Rect> gSpriteClips;
	int8_t	sprite_index;
	int8_t	sprite_change;

	Uint32 prevTime;

	std::shared_ptr<EImageResourceInfo>	sprite_base;
};
