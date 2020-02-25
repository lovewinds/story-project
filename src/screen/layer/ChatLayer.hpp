#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "screen/layer/ScreenLayer.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "graphic/texture/ImageTexture.hpp"
#include "graphic/texture/TextTexture.hpp"
#include "graphic/animation/EAnimation.hpp"

namespace story {
namespace Screen {

class ChatLayer : public ScreenLayer
{
public:
  ChatLayer(std::string name);
  virtual ~ChatLayer();

  /* Primitive Scene actions */
  virtual void handleEvent(SDL_Event e);
  virtual void render();
  virtual void update(Uint32 currentTime, Uint32 accumulator);

  void testAnimation(Graphic::AnimationState state);

protected:

};

} /* namespace Screen */
} /* namespace story */
