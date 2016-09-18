#pragma once
#include <list>
#include <string>

#include "Ecore.hpp"
#include "resource/EScene.hpp"

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

	void handleEvent(SDL_Event e);
	void render();
	void update(Uint32 currentTime, Uint32 accumulator);

	void temp_moveBackGround(double dx, double dy);
	void temp_moveCharacter(double dx, double dy);

	bool playScene(std::string scene_name);
	void stopCurrentScene();

	void pauseCurrentScene();
	void startCurrentScene();

	void initDebugScene();

private:
	std::shared_ptr<EScene> currentScene;
	std::shared_ptr<EScene> overlay;
	std::shared_ptr<EScene> debug_overlay;

	bool overlayState;

	int testState;
	int testBackgroundState;

	void propagateEvent(SDL_Event e);
};