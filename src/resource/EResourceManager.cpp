#include <SDL.h>

#include "resource/EResourceManager.h"
#include "resource/XMLResourceLoader.h"

EResourceManager::EResourceManager()
{
	loader = new XMLResourceLoader();
	SDL_assert(loader);
}

EResourceManager::~EResourceManager()
{
}

void EResourceManager::loadResource(const char* res_file)
{
	loader->loadScene(res_file);
}
