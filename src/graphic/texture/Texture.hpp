#pragma once

#include <SDL.h>
#include <memory>

#include "util/SDLWrap.hpp"

namespace story {
namespace Graphic {

class Texture
{
public:
  Texture();
  virtual ~Texture();

  /* Deallocates texture */
  virtual void dealloc();

  /* Set blending */
  void setBlendMode(SDL_BlendMode blending);

  /* Set alpha modulation */
  void setAlpha(Uint8 alpha);

  /* Set color modulation */
  void setColor(Uint8 red, Uint8 green, Uint8 blue);

  /* Position */
  double getPositionX();
  double getPositionY();
  void movePositionTo(double x, double y);
  void movePositionBy(double delta_x, double delta_y);
  int getWidth();
  int getHeight();
  void setWidth(double width, bool ratio = false);
  void setHeight(double height, bool ratio = false);

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0) = 0;
  virtual void render(
    int base_x, int base_y,
    int delta_x, int delta_y, double delta_angle) = 0;

protected:
  /* The actual SDL texture */
  std::shared_ptr<SDLTextureWrap> mTexture;

  /* Position */
  double p_x;
  double p_y;

  /* Texture dimensions */
  int mWidth = 0;
  int mHeight = 0;
  double wRatio = 1.0;
  double hRatio = 1.0;

  /* Renders texture at given point */
  virtual void texture_render(int x, int y,
    SDL_Rect* clip = NULL,
    double angle = 0.0,
    bool auto_size_by_dpi = true,
    SDL_Point* center = NULL,
    SDL_RendererFlip flip = SDL_FLIP_NONE);
};

} /* namespace Graphic */
} /* namespace story */
