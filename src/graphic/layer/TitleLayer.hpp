#pragma once

#include <string>
#include <SDL.h>

#include "graphic/layer/Layer.hpp"
#include "graphic/texture/ESprite.hpp"
#include "graphic/texture/EImageTexture.hpp"
#include "graphic/texture/ETextTexture.hpp"
#include "graphic/animation/EAnimation.hpp"

namespace story {
namespace Graphic {

class TitleLayer : public Layer
{
public:
  TitleLayer(std::string name);
  virtual ~TitleLayer();

  /* Primitive Scene actions */
  virtual void handleEvent(SDL_Event e);
  virtual void render();
  virtual void update(Uint32 currentTime, Uint32 accumulator);

protected:
  int menu_index = 0;

  void initMenuItem();
  void sampleMenuState(std::string id);
  bool checkMenuClicked(int x, int y);
};

} /* namespace Graphic */
} /* namespace story */
