#pragma once

#include <string>
#include <SDL.h>

class SceneManager;

class EScreenManager
{
public:
	EScreenManager();
	~EScreenManager();

	/* Prohibit copy constructor */
	EScreenManager(EScreenManager& self) = delete;

	bool playScene(std::string scene_name);
	void initDebugScene();

	/* Primitive functions */
	void render();
	void update(Uint32 currentTime, Uint32 accumulator);
	void handleEvent(SDL_Event e);

private:
	SceneManager* sceneManager;
};
