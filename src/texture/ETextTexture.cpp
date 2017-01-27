#include <SDL_ttf.h>

#include "Ecore.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EResourceFactory.hpp"
#include "texture/ETextTexture.hpp"
#include "util/LogHelper.hpp"

ETextTexture::ETextTexture(std::string text, SDL_Color textColor, SDL_Color bgColor)
 : EDrawable()
{
	this->message = text;
	this->bgColor = bgColor;
	this->textColor = textColor;

	_createTexture();
}

ETextTexture::~ETextTexture()
{
	_removeTexture();
}

void ETextTexture::_createTexture()
{
	TTF_Font* gFont = Ecore::getInstance()->getFont();
	SDL_Renderer* gRenderer = Ecore::getInstance()->getRenderer();
	EResourceFactory& resFactory = Ecore::getInstance()->getResourceFactory();

	if (mTexture) {
		mTexture.reset();
	}

	mTexture = resFactory.createTextTexture(message, textColor, bgColor);
	//LOG_ERR("[Text] texture [%p]", &mTexture);
	if (!mTexture) {
		LOG_ERR("Failed to allocate text texture!");
		return;
	}

	mWidth = mTexture->getWidth();
	mHeight = mTexture->getHeight();
}

void ETextTexture::_removeTexture()
{
	if (mTexture)
		mTexture.reset();
}

void ETextTexture::update(Uint32 currentTime, Uint32 accumulator)
{
}

void ETextTexture::setText(const std::string& text)
{
	static SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	static SDL_Color bgColor = { 0x0, 0x0, 0x0 };

	if (message.compare(text) != 0)
	{
		//LOG_INFO("****** Update String! [%s -> %s]", message.c_str(), text.c_str());
		message = text;
		_createTexture();
	}
}

void ETextTexture::render()
{
	int x, y;
	x = (animation) ? (int)(p_x + animation->getX()) : (int)p_x;
	y = (animation) ? (int)(p_y + animation->getY()) : (int)p_y;
	//texture_render(SCREEN_WIDTH - textTexture.getWidth() - 10, 10);
	if (mTexture) {
		//LOG_INFO("Text render [%s] (%d,%d)", message.c_str(), (int)p_x, (int)p_y);
		texture_render(x, y);
	}
}
