#include "Ecore.hpp"
#include "texture/EDrawable.hpp"
#include "util/LogHelper.hpp"

EDrawable::EDrawable() :
p_x(0), p_y(0)
{
	/* Initialize */
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
}

EDrawable::~EDrawable()
{
	dealloc();
}

void EDrawable::setBlendMode(SDL_BlendMode blending)
{
	/* Set blending function */
	SDL_SetTextureBlendMode(mTexture->getTexture(), blending);
}

void EDrawable::setAlpha(Uint8 alpha)
{
	/* Modulate texture alpha */
	SDL_SetTextureAlphaMod(mTexture->getTexture(), alpha);
}

void EDrawable::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	/* Modulate texture rgb */
	SDL_SetTextureColorMod(mTexture->getTexture(), red, green, blue);
}

void EDrawable::movePositionTo(double x, double y)
{
	p_x = x;
	p_y = y;
}

void EDrawable::movePositionBy(double delta_x, double delta_y)
{
	p_x += delta_x;
	p_y += delta_y;
}

void EDrawable::setAnimation(std::shared_ptr<EAnimation> animation)
{
	this->animation = animation;
}

std::shared_ptr<EAnimation> EDrawable::getAnimation()
{
	return animation;
}

AnimationState EDrawable::getAnimationState()
{
	if (nullptr == animation)
		return ANI_NONE;

	return animation->getState();
}

void EDrawable::startAnimation()
{
	if (animation) {
		animation->start();
	}
}

void EDrawable::stopAnimation()
{
	if (animation) {
		animation->stop();
	}
}

void EDrawable::pauseAnimation()
{
	if (animation) {
		animation->pause();
	}
}

void EDrawable::resumeAnimation()
{
	if (animation) {
		animation->resume();
	}
}

void EDrawable::finishedAnimationCallback(double delta_x, double delta_y)
{
	LOG_DBG("Animation Finished. update position");
	p_x += delta_x;
	p_y += delta_y;

	//animation = nullptr;
}

void EDrawable::syncAnimationCallback(double delta_x, double delta_y)
{
	LOG_DBG("update position");
	p_x += delta_x;
	p_y += delta_y;
}

void EDrawable::dealloc()
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

bool EDrawable::allocate()
{
	return true;
}

void EDrawable::deallocate()
{
}

void EDrawable::texture_render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
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

void EDrawable::texture_render_resize(int x, int y, SDL_Rect* clip, double ratio_w, double ratio_h, double angle, SDL_Point* center, SDL_RendererFlip flip)
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
	renderQuad.w = (int)(renderQuad.w * ratio_w);
	renderQuad.h = (int)(renderQuad.h * ratio_h);

	/* Render to screen */
	SDL_RenderCopyEx(gRenderer, mTexture->getTexture(), clip, &renderQuad, angle, center, flip);
}
