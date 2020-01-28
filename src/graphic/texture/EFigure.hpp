#pragma once

#include <SDL.h>

#include "graphic/texture/EDrawable.hpp"

namespace story {
namespace Graphic {

class EFigure : public EDrawable
{
public:
  EFigure(int x = 0, int y = 0, int width = 0, int height = 0,
      SDL_Color color = {0xFF, 0xFF, 0xFF});

  virtual ~EFigure();

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
  virtual void render(int delta_x, int delta_y, double delta_angle);

  /* Animation */
  //virtual void startAnimation();
  //virtual void stopAnimation();

protected:
  /* animation */
  bool animating;
  Uint32 startTime;

  /* drawing */
  int radian;
  int alpha;
  SDL_Color color;
};

} /* namespace Graphic */
} /* namespace story */
