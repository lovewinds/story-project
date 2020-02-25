#pragma once

#include <string>
#include <SDL.h>

#include "screen/layer/ScreenLayer.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "graphic/texture/ImageTexture.hpp"
#include "graphic/texture/TextTexture.hpp"
#include "graphic/animation/EAnimation.hpp"

namespace story {
namespace Screen {

class TitleLayer : public ScreenLayer
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

} /* namespace Screen */
} /* namespace story */
