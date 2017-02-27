#pragma once

#include <string>
#include <memory>
#include <vector>

#include "EDrawable.hpp"
#include "resource/ESpriteType.hpp"

class EImageResource;

class ESprite : public EDrawable
{
public:
	ESprite(std::string name, std::shared_ptr<ESpriteType> spriteType);
	~ESprite();

	std::string getName();
	void setIndex(unsigned int index);
	//void setControllable(bool controllable);
	//bool isControllable();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render(int delta_x, int delta_y, double delta_angle);

protected:
	ESprite();

	void _createTexture();
	void _removeTexture();

	std::string name;
	std::string base_image_name;

	std::vector<SDL_Rect> gSpriteClips;
	int8_t	sprite_index;
	int8_t	sprite_change;

	//bool controllable;

	Uint32 prevTime;

	std::shared_ptr<EImageResource>	sprite_base;
};
