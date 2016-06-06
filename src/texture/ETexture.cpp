#include "Ecore.hpp"
#include "texture/ETexture.h"
#include "util/LogHelper.hpp"

ETexture::ETexture() :
p_x(0), p_y(0)
{
	/* Initialize */
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
}

ETexture::~ETexture()
{
	dealloc();
}

void ETexture::setBlendMode(SDL_BlendMode blending)
{
	/* Set blending function */
	SDL_SetTextureBlendMode(mTexture->getTexture(), blending);
}

void ETexture::setAlpha(Uint8 alpha)
{
	/* Modulate texture alpha */
	SDL_SetTextureAlphaMod(mTexture->getTexture(), alpha);
}

void ETexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	/* Modulate texture rgb */
	SDL_SetTextureColorMod(mTexture->getTexture(), red, green, blue);
}

void ETexture::movePositionTo(double x, double y)
{
	p_x = x;
	p_y = y;
}

void ETexture::movePositionBy(double delta_x, double delta_y)
{
	p_x += delta_x;
	p_y += delta_y;
}

void ETexture::dealloc()
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

void ETexture::texture_render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	/* Set rendering space and render to screen */
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	/* Set clip rendering dimensions */
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	/* Render to screen */
	SDL_RenderCopyEx(gRenderer, mTexture->getTexture(), clip, &renderQuad, angle, center, flip);
}

void ETexture::texture_render_resize(int x, int y, SDL_Rect* clip, Uint8 expand, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	/* Set rendering space and render to screen */
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	/* Set clip rendering dimensions */
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	renderQuad.w *= expand;
	renderQuad.h *= expand;

	/* Render to screen */
	SDL_RenderCopyEx(gRenderer, mTexture->getTexture(), clip, &renderQuad, angle, center, flip);
}
