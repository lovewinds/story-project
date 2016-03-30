#pragma once

#include <string>
#include <SDL.h>

class ETextureHandler;

class EScreenManager
{
public:
	EScreenManager();
	~EScreenManager();

	/* Prohibit copy constructor */
	EScreenManager(EScreenManager& self) = delete;

	bool loadScene(std::string& scene_name);
	bool createBackgroundImage(std::string& image_path);
	bool createSprite(std::string& sprite_path);

	/* Primitive functions */
	void render();
	void update(Uint32 currentTime, Uint32 accumulator);
	void handleEvent(SDL_Event e);

private:
	ETextureHandler *textureHandler;
};
