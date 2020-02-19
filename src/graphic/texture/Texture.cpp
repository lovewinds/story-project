#include "core/Ecore.hpp"
#include "graphic/texture/Texture.hpp"
#include "util/LogHelper.hpp"

namespace story {
namespace Graphic {

Texture::Texture() :
p_x(0), p_y(0)
{
  /* Initialize */
  mTexture = nullptr;
  mWidth = 0;
  mHeight = 0;
}

Texture::~Texture()
{
  dealloc();
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
  /* Set blending function */
  SDL_SetTextureBlendMode(mTexture->getTexture(), blending);
}

void Texture::setAlpha(Uint8 alpha)
{
  /* Modulate texture alpha */
  SDL_SetTextureAlphaMod(mTexture->getTexture(), alpha);
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
  /* Modulate texture rgb */
  SDL_SetTextureColorMod(mTexture->getTexture(), red, green, blue);
}

void Texture::movePositionTo(double x, double y)
{
  p_x = x;
  p_y = y;
}

void Texture::movePositionBy(double delta_x, double delta_y)
{
  p_x += delta_x;
  p_y += delta_y;
}

double Texture::getPositionX()
{
  return p_x;
}

double Texture::getPositionY()
{
  return p_y;
}

int Texture::getWidth()
{
  return mWidth;
}

int Texture::getHeight()
{
  return mHeight;
}

void Texture::dealloc()
{
  /* Free texture if it exists */
  if (mTexture)
  {
    /* std::shared_ptr */
    //SDL_DestroyTexture(mTexture);
    //mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

void Texture::texture_render(int x, int y, SDL_Rect* clip, double angle,
  bool auto_size_by_dpi, SDL_Point* center, SDL_RendererFlip flip)
{
  SDL_Renderer *gRenderer = story::Core::Ecore::getInstance()->getRenderer();

  /* Set rendering space and render to screen */
  SDL_Rect renderQuad = { x, y, mWidth, mHeight };

  /* Set clip rendering dimensions */
  if (clip != NULL)
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  if (story::Core::Ecore::isHighDPI() == true)
  {
    renderQuad.x *= story::Core::Ecore::getDisplayScale();
    renderQuad.y *= story::Core::Ecore::getDisplayScale();
    if (auto_size_by_dpi) {
      renderQuad.w *= story::Core::Ecore::getDisplayScale();
      renderQuad.h *= story::Core::Ecore::getDisplayScale();
    }
  }

  /* Render to screen */
  SDL_RenderCopyEx(gRenderer, mTexture->getTexture(), clip, &renderQuad, angle, center, flip);
}

void Texture::texture_render_resize(int x, int y, SDL_Rect* clip,
  double ratio_w, double ratio_h, double angle,
  bool auto_size_by_dpi, SDL_Point* center, SDL_RendererFlip flip)
{
  SDL_Renderer *gRenderer = story::Core::Ecore::getInstance()->getRenderer();

  /* Set rendering space and render to screen */
  SDL_Rect renderQuad = { x, y, mWidth, mHeight };

  /* Set clip rendering dimensions */
  if (clip != NULL)
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  renderQuad.w = (int)(renderQuad.w * ratio_w);
  renderQuad.h = (int)(renderQuad.h * ratio_h);

  if (story::Core::Ecore::isHighDPI() == true)
  {
    renderQuad.x *= story::Core::Ecore::getDisplayScale();
    renderQuad.y *= story::Core::Ecore::getDisplayScale();
    if (auto_size_by_dpi) {
      renderQuad.w *= story::Core::Ecore::getDisplayScale();
      renderQuad.h *= story::Core::Ecore::getDisplayScale();
    }
  }

  /* Render to screen */
  SDL_RenderCopyEx(gRenderer, mTexture->getTexture(), clip, &renderQuad, angle, center, flip);
}

} /* namespace Graphic */
} /* namespace story */
