#pragma once

#include <string>

#include "util/SDLWrap.hpp"
#include "screen/layer/ScreenLayer.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "graphic/texture/ImageTexture.hpp"
#include "graphic/texture/TextTexture.hpp"
#include "graphic/animation/EAnimation.hpp"

namespace story {
namespace Screen {

class DbgOverlayLayer : public ScreenLayer
{
public:
  DbgOverlayLayer(std::string name);
  virtual ~DbgOverlayLayer();

  /* Primitive Scene actions */
  virtual void handleEvent(SDL_Event e);
  virtual void render();
  virtual void update(Uint32 currentTime, Uint32 accumulator);
};

} /* namespace Screen */
} /* namespace story */
