#pragma once
#include <list>
#include "Ecore.h"
#include "ETexture.h"

using std::list;

enum TextureType {
	TEXTURE_TYPE_FONT,
	TEXTURE_TYPE_PNG,
	TEXTURE_TYPE_RENDER,
};

class ETextureHandler
{
public:
	ETextureHandler();
	~ETextureHandler();

	static ETextureHandler* getInstance();

	void createTexture(int x, int y);
	void removeTexture();
	void handleEvent(SDL_Event e);

private:
	static ETextureHandler*	instance;
	list<ETexture*>	textureList;

	void propagateEvent(SDL_Event e);
};
