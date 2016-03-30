#include "Ecore.hpp"
#include "texture/ESprite.hpp"
#include "resource/EImageResourceInfo.hpp"
#include "util/LogHelper.hpp"

#define SPRITE_WIDTH  64
#define SPRITE_HEIGHT 96

ESprite::ESprite() :
	ETexture()
{
	sprite_index = 0;
	sprite_change = 1;

	/* No blending */
	setBlendMode(SDL_BLENDMODE_NONE);

	std::string name("test");
	std::string path("test_path");

	resource = std::shared_ptr<EImageResourceInfo>(new EImageResourceInfo(name, path));
	LOG_INFO("resource ref. count: [%lu]", resource.use_count());
}

ESprite::~ESprite()
{
}

bool ESprite::alloc(std::string path)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	/* Get rid of preallocated texture */
	dealloc();

	/* The final texture */
	SDL_Texture* newTexture = NULL;

	/* Load image at specified path */
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		LOG_INFO("Unable to load image %s! SDL_image Error: %s\n",
			path.c_str(), IMG_GetError());
	}
	else
	{
		/* Color key image */
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x00, 0xFF));

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

			gSpriteClips[0].x = 0;
			gSpriteClips[0].y = 0;
			gSpriteClips[0].w = SPRITE_WIDTH;
			gSpriteClips[0].h = SPRITE_HEIGHT;

			gSpriteClips[1].x = SPRITE_WIDTH * 1;
			gSpriteClips[1].y = 0;
			gSpriteClips[1].w = SPRITE_WIDTH;
			gSpriteClips[1].h = SPRITE_HEIGHT;

			gSpriteClips[2].x = SPRITE_WIDTH * 2;
			gSpriteClips[2].y = 0;
			gSpriteClips[2].w = SPRITE_WIDTH;
			gSpriteClips[2].h = SPRITE_HEIGHT;

			gSpriteClips[3].x = SPRITE_WIDTH * 3;
			gSpriteClips[3].y = 0;
			gSpriteClips[3].w = SPRITE_WIDTH;
			gSpriteClips[3].h = SPRITE_HEIGHT;
		}

		/* Get rid of old loaded surface */
		SDL_FreeSurface(loadedSurface);
	}

	/* Return success */
	mTexture = newTexture;
	return mTexture != NULL;
}

bool ESprite::loadFromFile(std::string path)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	/* Get rid of preallocated texture */
	dealloc();

	/* The final texture */
	SDL_Texture* newTexture = NULL;

	/* Load image at specified path */
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		LOG_INFO("Unable to load image %s! SDL_image Error: %s\n",
			path.c_str(), IMG_GetError());
	}
	else
	{
		/* Color key image */
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x00, 0xFF));

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

			gSpriteClips[0].x = 0;
			gSpriteClips[0].y = 0;
			gSpriteClips[0].w = SPRITE_WIDTH;
			gSpriteClips[0].h = SPRITE_HEIGHT;

			gSpriteClips[1].x = SPRITE_WIDTH * 1;
			gSpriteClips[1].y = 0;
			gSpriteClips[1].w = SPRITE_WIDTH;
			gSpriteClips[1].h = SPRITE_HEIGHT;

			gSpriteClips[2].x = SPRITE_WIDTH * 2;
			gSpriteClips[2].y = 0;
			gSpriteClips[2].w = SPRITE_WIDTH;
			gSpriteClips[2].h = SPRITE_HEIGHT;

			gSpriteClips[3].x = SPRITE_WIDTH * 3;
			gSpriteClips[3].y = 0;
			gSpriteClips[3].w = SPRITE_WIDTH;
			gSpriteClips[3].h = SPRITE_HEIGHT;
		}

		/* Get rid of old loaded surface */
		SDL_FreeSurface(loadedSurface);
	}

	/* Return success */
	mTexture = newTexture;
	return mTexture != NULL;
}

void ESprite::update(Uint32 currentTime, Uint32 accumulator)
{
	static Uint32 prevTime = 0;

	if (currentTime - prevTime > 400)
	{
		prevTime = currentTime;
#if 0
		sprite_index = sprite_index + sprite_change;
		if (sprite_index > 2) {
			sprite_index = 1;
			sprite_change = -1;
		}
		else if (sprite_index < 0) {
			sprite_index = 1;
			sprite_change = 1;
		}
#endif
		sprite_index++;
		if (sprite_index > 3)
			sprite_index = 0;
	}
}

void ESprite::render()
{
	//texture_render(200, 200, &gSpriteClips[sprite_index]);
	texture_render((int)p_x, (int)p_y, &gSpriteClips[sprite_index]);
	//texture_render_resize(100, 100, &gSpriteClips[sprite_index], 4);
}