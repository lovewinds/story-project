#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "texture/EImageTexture.hpp"

/* Screen dimension constants */
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

EImageTexture::EImageTexture(std::string path) :
	ETexture(),
	m_degrees(0.0)
{
	radian = 0;
	animating = false;
	image_path = path;
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
	dealloc();
}


bool EImageTexture::alloc()
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	/* Get rid of preallocated texture */
	dealloc();

	/* The final texture */
	SDL_Texture* newTexture = NULL;

	/* Load image at specified path */
	loadedSurface = IMG_Load(image_path.c_str());
	if (loadedSurface == NULL)
	{
		LOG_INFO("Unable to load image %s! SDL_image Error: %s\n",
				image_path.c_str(), IMG_GetError());
	}
	else
	{
		/* Color key image */
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		/* Create texture from surface pixels */
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			LOG_INFO("Unable to create texture from %s! SDL Error: %s\n",
					image_path.c_str(), SDL_GetError());
		}
		else
		{
			/* Get image dimensions */
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		/* Get rid of old loaded surface */
		SDL_FreeSurface(loadedSurface);
	}

	/* Return success */
	mTexture = newTexture;
	return mTexture != NULL;
}

bool EImageTexture::loadFromFile(std::string path)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	/* Get rid of preallocated texture */
	dealloc();

	/* The final texture */
	SDL_Texture* newTexture = NULL;

	/* Load image at specified path */
	loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		LOG_INFO("Unable to load image %s! SDL_image Error: %s\n",
				path.c_str(), IMG_GetError());
	}
	else
	{
		/* Color key image */
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		/* Create texture from surface pixels */
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			LOG_INFO("Unable to create texture from %s! SDL Error: %s\n",
					path.c_str(), SDL_GetError());
		}
		else
		{
			/* Get image dimensions */
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		/* Get rid of old loaded surface */
		SDL_FreeSurface(loadedSurface);
	}

	/* Return success */
	mTexture = newTexture;
	return mTexture != NULL;
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

	//texture_render(0, 0, &rect);
	texture_render((int)p_x, (int)p_y, &rect);
}

int EImageTexture::getWidth()
{
	return mWidth;
}

int EImageTexture::getHeight()
{
	return mHeight;
}
