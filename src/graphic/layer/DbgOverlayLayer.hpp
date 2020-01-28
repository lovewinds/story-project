#pragma once

#include <string>

#include "util/SDLWrap.hpp"
#include "graphic/layer/Layer.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"
#include "texture/ETextTexture.hpp"
#include "object/animation/EAnimation.hpp"

namespace story {
namespace Graphic {

class DbgOverlayLayer : public Layer
{
public:
	DbgOverlayLayer(std::string name);
	virtual ~DbgOverlayLayer();

	/* Primitive Scene actions */
	virtual void handleEvent(SDL_Event e);
	virtual void render();
	virtual void update(Uint32 currentTime, Uint32 accumulator);
};

} /* namespace Graphic */
} /* namespace story */
