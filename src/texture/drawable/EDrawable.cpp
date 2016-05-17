#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "texture/drawable/EDrawable.h"

EDrawable::EDrawable()
{
	/* Initialize */
	mTexture = NULL;
	animating = false;

	alpha = 255;
}

EDrawable::EDrawable(int x, int y)
{
	/* Initialize */
	mTexture = NULL;
	animating = false;

	/* Set Position */
	p_x = x;
	p_y = y;

	alpha = 255;
}

EDrawable::~EDrawable()
{

}

void EDrawable::animateStart(Uint32 start)
{
	animating = true;
	startTime = start;
}

void EDrawable::update(Uint32 currentTime, Uint32 accumulator)
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

void EDrawable::render()
{
	SDL_Renderer *renderer = Ecore::getInstance()->getRenderer();
	if (renderer == NULL)
		return;

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
}
