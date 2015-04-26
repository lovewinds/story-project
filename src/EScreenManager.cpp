#include "EScreenManager.h"

EScreenManager::EScreenManager()
{
	textureHandler = ETextureHandler::getInstance();
}

EScreenManager::~EScreenManager()
{
}

bool EScreenManager::loadMedia()
{
	bool	success = true;
#if 0
	/* Load arrow */
	if (!boxTexture.loadFromFile("../res/kachan.png"))
	{
		ERROR("Failed to load box texture!\n");
		success = false;
	}
#endif
	return success;
}

void EScreenManager::render(double d_fps)
{
	/* Render through TextureHandler */
	textureHandler->render(d_fps);
}

void EScreenManager::update(Uint32 currentTime, Uint32 accumulator)
{
	/* Update through TextureHandler */
	textureHandler->update(currentTime, accumulator);
}

void EScreenManager::handleEvent(SDL_Event e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		SDL_MouseButtonEvent *me = &e.button;
		textureHandler->handleEvent(e);
	}
}
