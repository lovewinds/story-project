#include <SDL.h>
//#include <SDL2_gfxPrimitives.h>

#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"
#include "graphic/texture/FigureTexture.hpp"

namespace story {
namespace Graphic {

FigureTexture::FigureTexture(int x, int y, int width, int height, SDL_Color color)
{
  /* Initialize */
  mTexture = nullptr;
  animating = false;

  /* Set Position */
  p_x = x;
  p_y = y;
  this->color = color;

  alpha = 255;

  mWidth = width;
  mHeight = height;
}

FigureTexture::~FigureTexture()
{

}

void FigureTexture::update(Uint32 currentTime, Uint32 accumulator)
{
  static Uint32 prevTime = 0;
  const Uint32 animationTime = 1000;
  const Uint32 depth = 16;

  Uint32 atomicTime = (currentTime - startTime);
  Uint32 delta = atomicTime - prevTime;
  /* 360 degree / 3000 * atomicTime */
  double degree = 360.0 / animationTime * atomicTime;
  int current_depth = 0;

  if (animating == false)
    return;

  if (degree >= 360.0 * depth) {
    /* infinite animation */
    alpha = 255;
    animating = false;
    LOG_INFO("FINISH");
    return;
  }
  radian = (int)degree;
  current_depth = (int)(degree / 360.0);

  /* change color */
  alpha = 256 - current_depth * 16;
  //alpha = current_depth * 16 + 16;
  if (alpha == 256)
    alpha = 255;
#if 0
  INFO("[0x%p] degree: %f, radian: %d, alpha: %d, depth: %d",
    this, degree, (int)(degree/360.0), alpha, current_depth);
#endif
  prevTime = delta;
}

void FigureTexture::render(
    int base_x, int base_y,
    int delta_x, int delta_y, double delta_angle)
{
  SDL_Renderer *renderer = story::Core::Ecore::getInstance()->getRenderer();
  if (renderer == NULL)
    return;

  // int width = story::Core::Ecore::getScreenWidth();
  // int height = story::Core::Ecore::getScreenHeight();

#if 0
  if (story::Core::Ecore::isHighDPI() == true) {
    auto n_x = p_x * story::Core::Ecore::getDisplayScale();
    auto n_y = p_y * story::Core::Ecore::getDisplayScale();
    auto n_width = (p_x + mWidth) * story::Core::Ecore::getDisplayScale();
    auto n_height = (p_y + mHeight) * story::Core::Ecore::getDisplayScale();
    boxRGBA(renderer, n_x, n_y, n_width, n_height,
      color.r, color.g, color.b, 0xAA );
  } else {
    boxRGBA(renderer, p_x, p_y, p_x + mWidth, p_y + mHeight,
      color.r, color.g, color.b, 0xAA );
  }
#endif

#if 0
  if (animating == false)
    return;
  if (alpha <= 240 && alpha > 0) {
    int b_alpha = alpha + 16;
    if (b_alpha > 255) b_alpha = 255;
  //}
#if 1
    filledCircleRGBA(renderer, p_x, p_y,
      30,
      0x70, 0xC6, 0xFF, b_alpha);
  }
  filledPieRGBA(renderer, p_x, p_y,
    30,
    270, radian + 270,
    0x70, 0xC6, 0xFF, alpha);
#endif
#endif
}

} /* namespace Graphic */
} /* namespace story */
