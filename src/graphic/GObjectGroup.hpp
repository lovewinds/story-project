#pragma once

#include <string>
#include <memory>
#include <map>

#include "graphic/GraphicObject.hpp"
#include "graphic/animation/EAnimation.hpp"
#include "graphic/texture/Texture.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "graphic/texture/ImageTexture.hpp"
#include "graphic/texture/TextTexture.hpp"

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
