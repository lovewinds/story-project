#include <SDL_ttf.h>

#include "core/Ecore.hpp"
#include "resource/ResourceManager.hpp"
#include "graphic/texture/TextTexture.hpp"
#include "util/LogHelper.hpp"

namespace story {
namespace Graphic {

TextTexture::TextTexture(std::string text,
    SDL_Color textColor, SDL_Color bgColor, int size)
 : Texture()
{
  this->message = text;
  this->bgColor = bgColor;
  this->textColor = textColor;
  this->size = size;

  _createTexture(size);
}

TextTexture::~TextTexture()
{
  _removeTexture();
}

void TextTexture::_createTexture(int size)
{
  story::Resource::ResourceManager& resManager =
    story::Core::Ecore::getInstance()->getResourceManager();

  if (story::Core::Ecore::isHighDPI() == true) {
    size *= story::Core::Ecore::getDisplayScale();
    wasHighDPI = true;
    prevScale = story::Core::Ecore::getDisplayScale();
  }

  font = resManager.getFont("NanumBarunpenR", size);
  if (nullptr == font) {
    LOG_ERR("Failed to get font[%s] size[%d]", "NanumBarunpenR", size);
    return;
  }

  if (mTexture) {
    LOG_DBG("Remove previous texture");
    mTexture.reset();
  }
  LOG_DBG("Create a new text texture [%s(%d)]", "NanumBarunpenR", size);

  mTexture = resManager.createTextTexture(message, textColor, bgColor, font);
  //LOG_ERR("[Text] texture [%p]", &mTexture);
  if (!mTexture) {
    LOG_ERR("Failed to allocate text texture!");
    return;
  }

  mWidth = mTexture->getWidth();
  mHeight = mTexture->getHeight();
}

void TextTexture::_removeTexture()
{
  if (mTexture)
    mTexture.reset();
}

void TextTexture::update(Uint32 currentTime, Uint32 accumulator)
{
  if (story::Core::Ecore::isHighDPI() != wasHighDPI) {
    wasHighDPI = story::Core::Ecore::isHighDPI();
    _createTexture(size);
  }
  else if (story::Core::Ecore::getDisplayScale() != prevScale) {
    prevScale = story::Core::Ecore::getDisplayScale();
    _createTexture(size);
  }
}

void TextTexture::setText(const std::string& text)
{
  if (message.compare(text) != 0)
  {
    //LOG_INFO("****** Update String! [%s -> %s]", message.c_str(), text.c_str());
    message = text;
    _createTexture(size);
  }
}

void TextTexture::render(
    int base_x, int base_y,
    int delta_x, int delta_y, double delta_angle)
{
  int x, y;
  x = base_x + (int)(p_x) + delta_x;
  y = base_y + (int)(p_y) + delta_y;
  //texture_render(SCREEN_WIDTH - textTexture.getWidth() - 10, 10);
  if (mTexture) {
    //LOG_INFO("Text render [%s] (%d,%d)", message.c_str(), (int)p_x, (int)p_y);

    /* Text texture doesn't requires auto-DPI calculation */
    texture_render(x, y, NULL, delta_angle, false);
  }
}

} /* namespace Graphic */
} /* namespace story */
