#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResource.hpp"

#include "texture/EImageTexture.hpp"

/* Screen dimension constants */
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

EImageTexture::EImageTexture(std::string name, std::string base_image) :
	EDrawable(),
	m_degrees(0.0),
	wRatio(1.0), hRatio(1.0)
{
	radian = 0;
	animating = false;

	this->name = name;
	this->base_image = base_image;

	_createTexture();
}

EImageTexture::~EImageTexture()
{
	/* Deallocate */
	_removeTexture();
}

void EImageTexture::_createTexture()
{
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	if (base_image.empty()) {
		LOG_ERR("base_image is empty !");
		return;
	}

	image = resManager.getImageResource(base_image);
	if (!image) {
		LOG_ERR("Failed to get image resource !");
		return;
	}

	if (mWidth == 0)
		mWidth = image->getWidth();
	if (mHeight == 0)
		mHeight = image->getHeight();

	LOG_DBG("Created. original image size [%d x %d]", mWidth, mHeight);

	/* Get shared texture from Resource Manager */
	mTexture = image->getTexture();
}

void EImageTexture::_removeTexture()
{
	mTexture.reset();
	if (nullptr != image)
		image->releaseTexture();
}

void EImageTexture::update(Uint32 currentTime, Uint32 accumulator)
{
	/* Animation */
	if (animation) {
		animation->update(currentTime, accumulator);
	}
}

void EImageTexture::render()
{
	SDL_Window* window = Ecore::getInstance()->getWindow();
	int width = 0, height = 0;
	SDL_Rect rect = { 0, };
	double ani_x = 0.0, ani_y = 0.0;

	SDL_GetWindowSize(window, &width, &height);
	//rect.w = width;
	//rect.h = height;
	rect.w = mWidth;
	rect.h = mHeight;

	if (animation) {
		ani_x = animation->getX();
		ani_y = animation->getY();
	}

	if (mTexture) {
		//texture_render(0, 0, &rect);
		//texture_render((int)p_x, (int)p_y, &rect);
		if (wRatio != 1.0 || hRatio != 1.0)
			texture_render_resize(
				(int)(p_x + ani_x), (int)(p_y + ani_y),
				&rect, wRatio, hRatio);
		else
			texture_render((int)(p_x + ani_x), (int)(p_y + ani_y), &rect);
	}
	else
		LOG_ERR("Texture not exist !");
}

int EImageTexture::getWidth()
{
	return mWidth;
}

int EImageTexture::getHeight()
{
	return mHeight;
}

void EImageTexture::setWidth(double width, bool ratio)
{
	if (ratio)
		wRatio = width / 100.0;
	else
		mWidth = width;
}

void EImageTexture::setHeight(double height, bool ratio)
{
	if (ratio)
		hRatio = height / 100.0;
	else
		mHeight = height;
}

void EImageTexture::setPos(int x, int y)
{
	p_x = x;
	p_y = y;
}

std::string EImageTexture::getName()
{
	return name;
}
