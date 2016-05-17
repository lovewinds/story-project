#pragma once

#include <string>
#include <SDL.h>

class ESceneManager;

class EScreenManager
{
public:
	EScreenManager();
	~EScreenManager();

	/* Prohibit copy constructor */
	EScreenManager(EScreenManager& self) = delete;

	bool playScene(std::string scene_name);

	/* Primitive functions */
	void render();
	void update(Uint32 currentTime, Uint32 accumulator);
	void handleEvent(SDL_Event e);

private:
	ESceneManager* sceneManager;
};
