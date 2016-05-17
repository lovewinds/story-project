#pragma once
#include <list>
#include <string>

#include "Ecore.hpp"
#include "resource/ESceneInfo.hpp"
#include "texture/drawable/EDrawable.h"
#include "texture/ETexture.h"
#include "texture/ETextTexture.h"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"

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

class ESceneManager
{
public:
	ESceneManager();
	~ESceneManager();

	//static ESceneManager* getInstance();
	//static void setInstance(ESceneManager* handler);

	void createTexture(int x, int y);
	void removeTexture();
	void handleEvent(SDL_Event e);
	void render();
	void update(Uint32 currentTime, Uint32 accumulator);

	//bool createBackgroundImage(std::string img_path);
	//bool createSprite(std::string sprite_path);

	void temp_moveBackGround(double dx, double dy);
	void temp_moveCharacter(double dx, double dy);

	bool playScene(std::string scene_name);

private:
	//static ESceneManager*	instance;
	//std::list<EDrawable*>	textureList;
	std::shared_ptr<ESceneInfo> currentScene;

	//ETextTexture *textTexture;
	//ESprite* sprite;
	//EImageTexture* background;
	int testState;
	int testBackgroundState;

	void propagateEvent(SDL_Event e);
};
