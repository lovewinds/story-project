#include <SDL_ttf.h>

#include "Ecore.hpp"
#include "texture/ETextTexture.h"
#include "util/LogHelper.hpp"

ETextTexture::ETextTexture()
 : ETexture()
{
}


ETextTexture::~ETextTexture()
{
}

bool ETextTexture::loadFromRenderedText(const std::string& textureText, SDL_Color textColor, SDL_Color bgColor)
{
#if 0
	TTF_Font* gFont = Ecore::getInstance()->getFont();
	SDL_Renderer* gRenderer = Ecore::getInstance()->getRenderer();

	/* Get rid of preexisting texture */
	dealloc();

	/* Render text surface */
	//SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(gFont, textureText.c_str(), textColor, bgColor);
	SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		LOG_ERR("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		/* Get image dimensions */
		mWidth = textSurface->w;
		mHeight = textSurface->h;

		/* Create texture from surface pixels */
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			LOG_ERR("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}

		/* Get rid of old surface */
		SDL_FreeSurface(textSurface);
	}

	/* Return success */
	return mTexture != NULL;
#endif
	return false;
}


void ETextTexture::update(Uint32 currentTime, Uint32 accumulator)
{
	/* Handle Text Texture */
	static char str[256] = { 0, };

	/* Update Text */
	double d_fps = Ecore::getInstance()->GetFPS();
	SDL_snprintf(str, 256, "FPS: %0.2f", d_fps);

	update_string(str);
}

void ETextTexture::update_string(const std::string& str)
{
	static SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	static SDL_Color bgColor = { 0x0, 0x0, 0x0 };

	if (message.compare(str) != 0)
	{
		//INFO("****** Update String! [%s -> %s]", message.c_str(), str.c_str());
		message = str;

		/* Render Text */
		if (loadFromRenderedText(message, textColor, bgColor))
		{
			LOG_ERR("Failed to render text texture!");
		}
	}
}

void ETextTexture::render()
{
	//textTexture.render(SCREEN_WIDTH - textTexture.getWidth() - 10, 10);
	texture_render(50, 10);
}
