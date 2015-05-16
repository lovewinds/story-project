#include "Ecore.h"
#include "drawable\ESprite.h"

ESprite::ESprite() :
	ETexture()
{
	sprite_index = 0;
	sprite_change = 1;
}

ESprite::~ESprite()
{
}

bool ESprite::loadFromFile(std::string path)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	/* Get rid of preexisting texture */
	free();

	/* The final texture */
	SDL_Texture* newTexture = NULL;

	/* Load image at specified path */
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		INFO("Unable to load image %s! SDL_image Error: %s\n",
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
			INFO("Unable to create texture from %s! SDL Error: %s\n",
				path.c_str(), SDL_GetError());
		}
		else
		{
			/* Get image dimensions */
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;

			gSpriteClips[0].x = 0;
			gSpriteClips[0].y = 0;
			gSpriteClips[0].w = 32;
			gSpriteClips[0].h = 32;

			gSpriteClips[1].x = 32 * 1;
			gSpriteClips[1].y = 0;
			gSpriteClips[1].w = 32;
			gSpriteClips[1].h = 32;

			gSpriteClips[2].x = 32 * 2;
			gSpriteClips[2].y = 0;
			gSpriteClips[2].w = 32;
			gSpriteClips[2].h = 32;
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

		sprite_index = sprite_index + sprite_change;
		if (sprite_index > 2) {
			sprite_index = 1;
			sprite_change = -1;
		}
		else if (sprite_index < 0) {
			sprite_index = 1;
			sprite_change = 1;
		}
	}
}

void ESprite::render()
{
	texture_render(100, 100, &gSpriteClips[sprite_index]);
	//texture_render_resize(100, 100, &gSpriteClips[sprite_index], 4);
}