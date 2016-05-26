#include <SDL_ttf.h>

#include "Ecore.hpp"
#include "resource/EResourceManager.hpp"
#include "texture/ETextTexture.hpp"
#include "util/LogHelper.hpp"

ETextTexture::ETextTexture(std::string text, SDL_Color textColor, SDL_Color bgColor)
 : ETexture()
{
	this->message = text;
	this->bgColor = bgColor;
	this->textColor = textColor;
}


ETextTexture::~ETextTexture()
{
}

bool ETextTexture::allocate()
{
	TTF_Font* gFont = Ecore::getInstance()->getFont();
	SDL_Renderer* gRenderer = Ecore::getInstance()->getRenderer();
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();

	if (mTexture) {
		mTexture.reset();
	}

	mTexture = resManager.allocateTextTexture(message, textColor, bgColor);
	LOG_ERR("[Text] texture [%p]", mTexture);
	if (!mTexture) {
		LOG_ERR("Failed to allocate text texture!");
		return false;
	}

	mWidth = mTexture->getWidth();
	mHeight = mTexture->getHeight();
	return true;
}

void ETextTexture::deallocate()
{
	if (mTexture)
		mTexture.reset();
}

void ETextTexture::update(Uint32 currentTime, Uint32 accumulator)
{
	/* Handle Text Texture */
	static char str[256] = { 0, };

	/* Update Text */
	double d_fps = Ecore::getInstance()->GetFPS();
	SDL_snprintf(str, 256, "FPS: %0.2f", d_fps);

	setText(str);
}

void ETextTexture::setText(const std::string& text)
{
	static SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	static SDL_Color bgColor = { 0x0, 0x0, 0x0 };

	if (message.compare(text) != 0)
	{
		//LOG_INFO("****** Update String! [%s -> %s]", message.c_str(), text.c_str());
		message = text;
		allocate();
	}
}

void ETextTexture::render()
{
	//texture_render(SCREEN_WIDTH - textTexture.getWidth() - 10, 10);
	if (mTexture) {
		//LOG_INFO("Text render [%s] (%d,%d)", message.c_str(), (int)p_x, (int)p_y);
		texture_render(30, 30);
	}
}
