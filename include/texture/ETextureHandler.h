#pragma once
#include <list>
#include <string>

#include "Ecore.hpp"
#include "ETexture.h"
#include "drawable/EDrawable.h"
#include "ETextTexture.h"
#include "ESprite.hpp"
#include "EImageTexture.hpp"

using std::list;

enum TextureType {
	TEXTURE_TYPE_FONT,
	TEXTURE_TYPE_PNG,
	TEXTURE_TYPE_RENDER,
};

enum MoveDirection {
	DIR_STOP = 0x0,
	DIR_UP = 0x1000,
	DIR_RIGHT = 0x0100,
	DIR_DOWN = 0x0010,
	DIR_LEFT = 0x0001,
	DIR_ALL = 0x1111,
};

class ETextureHandler
{
public:
	ETextureHandler();
	~ETextureHandler();

	//static ETextureHandler* getInstance();
	//static void setInstance(ETextureHandler* handler);

	void createTexture(int x, int y);
	void removeTexture();
	void handleEvent(SDL_Event e);
	void render();
	void update(Uint32 currentTime, Uint32 accumulator);

	bool createBackgroundImage(std::string& img_path);
	bool createSprite(std::string& sprite_path);

	void temp_moveBackGround(double dx, double dy);
	void temp_moveCharacter(double dx, double dy);

private:
	static ETextureHandler*	instance;
	//list<ETexture*>	textureList;
	list<EDrawable*>	textureList;

	ETextTexture *textTexture;
	ESprite* sprite;
	EImageTexture* background;
	int testState;
	int testBackgroundState;

	void propagateEvent(SDL_Event e);
};
