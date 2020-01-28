#pragma once

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "graphic/texture/EDrawable.hpp"
#include "util/SDLWrap.hpp"

namespace story {
namespace Graphic {

class ETextTexture : public EDrawable
{
public:
  ETextTexture(std::string text, SDL_Color textColor, SDL_Color bgColor, int size = 28);
  virtual ~ETextTexture();

  /* Update String */
  virtual void setText(const std::string& text);

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
  virtual void render(int delta_x, int delta_y, double delta_angle);

protected:
  void _createTexture(int size);
  void _removeTexture();

  /* String to show */
  std::string message;

  SDL_Color textColor;
  SDL_Color bgColor;
  int size;

  /* Support dynamic DPI */
  bool wasHighDPI = false;
  float prevScale= 1.0f;
  std::shared_ptr<SDLFontWrap> font;
};

} /* namespace Graphic */
} /* namespace story */
