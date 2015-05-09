#include "drawable/EDrawable.h"

#include "Ecore.h"

EDrawable::EDrawable() :
m_x(0), m_y(0),
p_x(300), p_y(300)
{
	/* Initialize */
	mTexture = NULL;
	animating = false;

	alpha = 255;
}

EDrawable::EDrawable(int x, int y) :
m_x(0), m_y(0),
p_x(0), p_y(0)
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

void EDrawable::setBlendMode(SDL_BlendMode blending)
{
	/* Set blending function */
	SDL_SetTextureBlendMode(mTexture, blending);
}

void EDrawable::setAlpha(Uint8 alpha)
{
	/* Modulate texture alpha */
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void EDrawable::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	/* Modulate texture rgb */
	SDL_SetTextureColorMod(mTexture, red, green, blue);
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
		alpha = 0;
		animating = false;
		INFO("FINISH");
		return;
	}
	radian = degree;
	current_depth = degree / 360.0;

	/* change color */
	alpha = 256 - current_depth * 16;
	if (alpha == 256)
		alpha = 255;
#if 1
	INFO("[0x%p] degree: %f, radian: %d, alpha: %d, depth: %d",
		this, degree, (int)(degree/360.0), alpha, current_depth);
#endif
	prevTime = delta;
}

void EDrawable::draw(Uint32 currentTime, Uint32 accumulator)
{
	SDL_Renderer *renderer = Ecore::getInstance()->getRenderer();
	if (renderer == NULL)
		return;

	if (animating == false)
		return;
	if (alpha <= 240 && alpha > 0) {
		int b_alpha = alpha + 16;
		if (b_alpha > 255) b_alpha = 255;
		filledCircleRGBA(renderer, p_x, p_y,
			30,
			0x70, 0xC6, 0xFF, b_alpha);
	}
	filledPieRGBA(renderer, p_x, p_y,
		30,
		0, radian,
		0x70, 0xC6, 0xFF, alpha);
}