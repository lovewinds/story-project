#pragma once
#include <list>
#include "Ecore.h"
#include "ETexture.h"
#include "drawable/EDrawable.h"
#include "ETextTexture.h"
#include "ESprite.h"
#include "EImageTexture.h"

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

	void temp_moveBackGround(double dx, double dy);
	void temp_moveCharacter(double dx, double dy);

private:
	static ETextureHandler*	instance;
	//list<ETexture*>	textureList;
	list<EDrawable*>	textureList;

	ETextTexture *textTexture;
	ESprite* sprite;
	EImageTexture* background;

	void propagateEvent(SDL_Event e);
};
