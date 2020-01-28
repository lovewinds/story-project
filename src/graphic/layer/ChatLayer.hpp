#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "graphic/layer/Layer.hpp"
#include "graphic/texture/ESprite.hpp"
#include "graphic/texture/EImageTexture.hpp"
#include "graphic/texture/ETextTexture.hpp"
#include "graphic/animation/EAnimation.hpp"

namespace story {
namespace Graphic {

class ChatLayer : public Layer
{
public:
  ChatLayer(std::string name);
  virtual ~ChatLayer();

  /* Primitive Scene actions */
  virtual void handleEvent(SDL_Event e);
  virtual void render();
  virtual void update(Uint32 currentTime, Uint32 accumulator);

  void testAnimation(AnimationState state);

protected:

};

} /* namespace Graphic */
} /* namespace story */
