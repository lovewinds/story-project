#pragma once

#include <string>
#include <memory>
#include <vector>

#include "graphic/texture/EDrawable.hpp"
#include "resource/ESpriteType.hpp"

namespace story {
namespace Resource {
	class EImageResource;
}
}

namespace story {
namespace Graphic {

class ESprite : public EDrawable
{
public:
	ESprite(std::string name, std::shared_ptr<story::Resource::ESpriteType> spriteType);
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

	std::shared_ptr<story::Resource::EImageResource> sprite_base;
};

} /* namespace Graphic */
} /* namespace story */
