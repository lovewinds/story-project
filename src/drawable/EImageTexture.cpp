#include "Ecore.h"
#include "drawable/EImageTexture.h"

/* Screen dimension constants */
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

EImageTexture::EImageTexture() :
	ETexture(),
	m_degrees(0.0)
{
	radian = 0;
	animating = false;
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
	free();
}

bool EImageTexture::loadFromFile(std::string path)
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
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

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
	Uint32 compensatedTime = currentTime;// +accumulator;
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
	/* Previous state */
	//p_x = m_x;
	//p_y = m_y;

	Uint32 atomicTime = (compensatedTime - startTime);
	Uint32 delta = atomicTime - prevTime;
	
	if (atomicTime == 0) {
		velo = 1.0;
		accel = 0.0;
		//m_x = -562;
	}

	if (!animating)	return;
	if (atomicTime > 1000) {
		animating = false;
	}

	accel = 0.0;
	velo = 0.1;

	m_x = p_x;
	m_y = p_y + velo * ((atomicTime > 375) ? 375 : atomicTime);

	if (atomicTime > 375) {
		delta = atomicTime - 375;
		accel = -0.00026;
		//velo = 1.0 - (1.0 / 375.0) * (delta - 375);
		velo += accel * delta;
		m_y += velo * delta;
	}
	//m_x = (velo * delta) - 562;
	
	//m_x = (int)((compensatedTime - startTime) * velo) % 700;
	//m_x = (m_x * alpha) + (p_x * (1.0 - alpha));
#if 0
	INFO("delta: [%d] / v: [%f] / a: [%f] / Y: [%f] / Yd: [%f]",
		delta, velo, accel, m_y, m_y - p_y);
#endif

	prevTime = delta;
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

void EImageTexture::render()
{
	SDL_Window* window = Ecore::getInstance()->getWindow();
	int width = 0, height = 0;
	SDL_Rect rect = { 0, };

	SDL_GetWindowSize(window, &width, &height);
	rect.w = width;
	rect.h = height;

	texture_render(0, 0, &rect);
}

#if 0
void EImageTexture::paint(Uint32 currentTime, Uint32 accumulator)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	/* Set rendering space and render to screen */
	SDL_Rect renderQuad = { m_x, m_y, mWidth, mHeight };

	/*
	boxRGBA(gRenderer, m_x, 60, m_x + 200, 60 + 500, 0x11, 0x53, 0xDA, 0xFF);
	boxRGBA(gRenderer, m_x, 70, m_x + 190, 70 + 60, 0xFF, 0xFF, 0xFF, 0xFF);
	boxRGBA(gRenderer, m_x, 130, m_x + 190, 130 + 60, 0xFF, 0xFF, 0xFF, 0xAA);
	boxRGBA(gRenderer, m_x, 190, m_x + 190, 190 + 60, 0xFF, 0xFF, 0xFF, 0x88);
	boxRGBA(gRenderer, m_x, 250, m_x + 190, 250 + 60, 0xFF, 0xFF, 0xFF, 0x22);
	*/

	const Sint16 THICK = 4;
	const Sint16 WIDTH = 40;
	boxRGBA(gRenderer,
		m_x, m_y,
		m_x + WIDTH, m_y + WIDTH,
		0xFF, 0xFF, 0xFF, 0x30);
	boxRGBA(gRenderer,
		m_x + THICK, m_y + THICK,
		m_x + WIDTH - THICK, m_y + WIDTH - THICK,
		0x1B, 0x40, 0x5E, 0xFF);
}
#endif

int EImageTexture::getWidth()
{
	return mWidth;
}

int EImageTexture::getHeight()
{
	return mHeight;
}