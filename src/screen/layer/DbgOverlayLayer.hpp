#pragma once

#include <string>

#include "util/SDLWrap.hpp"
#include "graphic/layer/Layer.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "graphic/texture/ImageTexture.hpp"
#include "graphic/texture/TextTexture.hpp"
#include "graphic/animation/EAnimation.hpp"

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
