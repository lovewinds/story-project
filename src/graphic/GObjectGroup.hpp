#pragma once

#include <string>
#include <memory>
#include <map>

#include "object/animation/EAnimation.hpp"
#include "texture/EDrawable.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"
#include "texture/ETextTexture.hpp"

namespace story {
namespace Graphic {

class ObjectGroup : public Object
{
public:
	ObjectGroup();
	virtual ~ObjectGroup();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

	/* Primitive */

};

} /* namespace Graphic */
} /* namespace story */
