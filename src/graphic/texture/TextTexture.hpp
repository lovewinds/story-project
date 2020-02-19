#pragma once

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "graphic/texture/Texture.hpp"
#include "util/SDLWrap.hpp"

namespace story {
namespace Graphic {

class TextTexture : public Texture
{
public:
  TextTexture(std::string text, SDL_Color textColor, SDL_Color bgColor, int size = 28);
  virtual ~TextTexture();

  /* Update String */
  virtual void setText(const std::string& text);

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
  virtual void render(
    int base_x, int base_y,
    int delta_x, int delta_y, double delta_angle);

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
