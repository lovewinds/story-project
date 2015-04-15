#include "EScreenManager.h"


EScreenManager::EScreenManager():
boxTexture(),
textTexture()
{
}

EScreenManager::~EScreenManager()
{
	/* Free loaded images */
	boxTexture.free();

	/* Free fonts */
	textTexture.free();
}

bool EScreenManager::loadMedia()
{
	bool	success = true;
	//Load arrow
	if (!boxTexture.loadFromFile("../res/kachan.png"))
	{
		//printf("Failed to load box texture!\n");
		ERROR("Failed to load box texture!\n");
		success = false;
	}

	return success;
}

void EScreenManager::start(Uint32 prevTime)
{
	textTexture.animateStart(prevTime);
	boxTexture.animateStart(prevTime);
}

void EScreenManager::render(double d_fps)
{
	static char str[256] = { 0, };
	// Render arrow
	boxTexture.draw();

	// Render Text
	static SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	static SDL_Color bgColor = { 0x0, 0x0, 0x0 };
	SDL_snprintf(str, 256, "FPS: %0.2f", d_fps);
	if (!textTexture.loadFromRenderedText(str, textColor, bgColor))
	{
		//printf("Failed to render text texture!\n");
		ERROR("Failed to render text texture!\n");
	}
	//textTexture.render(SCREEN_WIDTH - textTexture.getWidth() - 10, 10);
	textTexture.render(50, 10);
}

void EScreenManager::update(Uint32 currentTime, Uint32 accumulator)
{
	boxTexture.calculate(currentTime, accumulator);
}

void EScreenManager::handleEvent(SDL_Event e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		boxTexture.animateStart(SDL_GetTicks());
		textTexture.animateStart(SDL_GetTicks());
	}
}