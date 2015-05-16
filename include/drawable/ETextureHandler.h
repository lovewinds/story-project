#pragma once
#include <list>
#include "Ecore.h"
#include "ETexture.h"
#include "EDrawable.h"
#include "ETextTexture.h"
#include "ESprite.h"

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
	void render();
	void update(Uint32 currentTime, Uint32 accumulator);

private:
	static ETextureHandler*	instance;
	//list<ETexture*>	textureList;
	list<EDrawable*>	textureList;

	ETextTexture *textTexture;
	ESprite* sprite;

	void propagateEvent(SDL_Event e);
};
