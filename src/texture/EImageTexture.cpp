#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResourceInfo.hpp"
#include "texture/EImageTexture.hpp"

#include <SDL.h>

/* Screen dimension constants */
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

EImageTexture::EImageTexture(std::string name, std::string base_image) :
	ETexture(),
	m_degrees(0.0)
{
	radian = 0;
	animating = false;
	this->name = name;
	this->base_image = base_image;
}

EImageTexture::EImageTexture(int x, int y) :
	ETexture(),
	m_degrees(0.0)
{
	radian = 0;
	animating = false;

	/* Set Position */
	p_x = x;
	p_y = y;
}

EImageTexture::~EImageTexture()
{
	/* Deallocate */
	deallocate();
}

bool EImageTexture::allocate()
{
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	if (base_image.empty()) {
		LOG_ERR("base_image is empty !");
		return false;
	}

	image = resManager.getImageResource(base_image);

	if (!image)
		return false;

	/* Get shared texture from Resource Manager */
	mTexture = image->getTexture();
	return true;
}

void EImageTexture::deallocate()
{
	mTexture.reset();
	image->releaseTexture();
}

void EImageTexture::update(Uint32 currentTime, Uint32 accumulator)
{
	static double dir = 1.0f;
	static unsigned int count = 0;
	static double accel = 0.026;
	static double velo = 1.0;

	static Uint32 prevTime = 0;
	static const Uint32 startTime = currentTime;
	Uint32 compensatedTime = currentTime;
	Uint32 atomicTime = (compensatedTime - startTime);
	Uint32 delta = atomicTime - prevTime;
	static double prev_y = 0;

	if (atomicTime == 0) {
		velo = 1.0;
		accel = 0.0;
	}

	if (!animating)	return;
	if (atomicTime > 1000) {
		animating = false;
	}

	accel = 0.0;
	velo = 0.1;

	prev_y = p_y + velo * ((atomicTime > 375) ? 375 : atomicTime);

	if (atomicTime > 375) {
		delta = atomicTime - 375;
		accel = -0.00026;
		//velo = 1.0 - (1.0 / 375.0) * (delta - 375);
		velo += accel * delta;
		prev_y += velo * delta;
	}

	prevTime = delta;
}

void EImageTexture::render()
{
	SDL_Window* window = Ecore::getInstance()->getWindow();
	int width = 0, height = 0;
	SDL_Rect rect = { 0, };

	SDL_GetWindowSize(window, &width, &height);
	rect.w = width;
	rect.h = height;

	if (mTexture)
		texture_render(0, 0, &rect);
		//texture_render((int)p_x, (int)p_y, &rect);
}

int EImageTexture::getWidth()
{
	return mWidth;
}

int EImageTexture::getHeight()
{
	return mHeight;
}

std::string EImageTexture::getName()
{
	return name;
}
