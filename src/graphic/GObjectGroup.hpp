#pragma once

#include <string>
#include <memory>
#include <map>

#include "graphic/GraphicObject.hpp"
#include "graphic/animation/EAnimation.hpp"
#include "graphic/texture/EDrawable.hpp"
#include "graphic/texture/ESprite.hpp"
#include "graphic/texture/EImageTexture.hpp"
#include "graphic/texture/ETextTexture.hpp"

namespace story {
namespace Graphic {

class ObjectGroup : public story::Graphic::Object
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
