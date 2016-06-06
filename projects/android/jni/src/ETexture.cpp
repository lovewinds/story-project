#include "ETexture.h"
#include "Ecore.h"

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

ETexture::ETexture(Ecore *core)
:core(NULL),
m_x(-562), m_y(0),
force_x(0), force_y(0),
m_degrees(0.0)
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;

	this->core = core;
}

ETexture::~ETexture()
{
	//Deallocate
	free();
}

bool ETexture::loadFromFile(std::string path)
{
	SDL_Renderer *gRenderer = core->getRenderer();

	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void ETexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void ETexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void ETexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void ETexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void ETexture::calculate(Uint32 currentTime)
{
	static double dir = 1.0f;
	static unsigned int count = 0;
	static double accel = 0.0f;
	static double velo = 0.1f;

	static unsigned int prevTime = 0;
	static const Uint32 startTime = currentTime;
#if 0
	if (count == 180) {
		accel = 0.0f;
		count = 0;
		dir = dir * -1.0f;

		velo = dir * -1.0f * 4.0f;
	}
	else if (count >= 160) {
		velo = 0;
	}
	else if (count > 150) {
		accel = dir * 0.8f;
	}
#endif
#if 0
	if (dir > 0) {
		if (m_x >= -1.0f * (0.1f * mWidth)) {
			accel = -1.0f * dir * (0.1*mWidth * 2);
		}
		else if (m_x >= 0) {
			velo = 0.0f;
			accel = 0.0f;
			count++;
			if (count > 30) {
				/* Change direction */
				dir *= -1.0f;
				count = 0;

				velo = -5.0f;
			}
		}
	}
	/* Reverse direction */
	else {
		if (m_x < -1.0*mWidth) {
			velo = 0.0f;
			accel = 0.0f;
			count++;
			if (count > 10) {
				/* Change direction */
				dir = dir * -1.0f;
				count = 0;

				velo = 8.0f;
			}
		}
	}
#endif
	// Move Texture
	// s = v * t
	// v = a * t
	// v = v0 + a * t
	// s = a * t * t
	// S = s0 + v*t
#if 0
	velo = velo + (accel * deltaTime / 100);
	m_x += velo * (deltaTime / 10);
#endif
	//m_x += 0.5;//frameTime;// / 10.0;
	double p_x = m_x;
	m_x = (int)((currentTime - startTime) * velo) % 700;
	//m_x = (m_x * alpha) + (p_x * (1.0 - alpha));
#if 0
	INFO("@: [%f] / Frame: %d / cal: %f / X : [%f -> %f]",
		alpha,
		currentTime, (currentTime - startTime) * velo, p_x, m_x);
	if (SDL_abs((int)(p_x - m_x)) > 1)
		INFO("!!!!!!!!!!!!!!!!!!");
#endif
#if 0
	if (m_x > 800.0)
		m_x -= 1600.0;
#endif
	//if (m_y < 0 || m_y > SCREEN_HEIGHT - 16)

	// Rotate
	//m_degrees += 2.0f;
}

void ETexture::draw()
{
	int f_x = SDL_floor(m_x + 0.5);
	INFO("X : %f / Flooring : %d", m_x, f_x);
	render(f_x, m_y,
		NULL, m_degrees,
		NULL, SDL_FLIP_NONE);
}

void ETexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Renderer *gRenderer = core->getRenderer();

	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void ETexture::render_resize(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Renderer *gRenderer = core->getRenderer();

	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = 16;
		renderQuad.h = 16;
	}
	renderQuad.w = 32;
	renderQuad.h = 32;

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

bool ETexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Color bgColor)
{
	TTF_Font* gFont = core->getFont();
	SDL_Renderer* gRenderer = core->getRenderer();
	//Get rid of preexisting texture
	free();

	//Render text surface
	//SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	SDL_Surface* textSurface = TTF_RenderText_Shaded(gFont, textureText.c_str(), textColor, bgColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

int ETexture::getWidth()
{
	return mWidth;
}

int ETexture::getHeight()
{
	return mHeight;
}