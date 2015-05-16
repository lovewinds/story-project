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

void EScreenManager::render()
{
	/* Render through TextureHandler */
	textureHandler->render();
}

void EScreenManager::update(Uint32 currentTime, Uint32 accumulator)
{
	/* Update through TextureHandler */
	textureHandler->update(currentTime, accumulator);
}

void EScreenManager::handleEvent(SDL_Event e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN
		|| e.type == SDL_FINGERDOWN
		|| e.type == SDL_FINGERMOTION)
	{
		textureHandler->handleEvent(e);
	}
}
