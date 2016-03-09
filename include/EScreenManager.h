#pragma once
#include "Ecore.h"
#include "texture/ETexture.h"
#include "texture/ETextureHandler.h"

class ETextureHandler;

class EScreenManager
{
public:
	EScreenManager();
	~EScreenManager();

	/* Initialize phase */
	bool loadResources(std::string& path);

	/* Primitive functions */
	void render();
	void update(Uint32 currentTime, Uint32 accumulator);
	void handleEvent(SDL_Event e);

private:
	ETextureHandler *textureHandler;
};
