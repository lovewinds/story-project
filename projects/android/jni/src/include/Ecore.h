#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "ETexture.h"

#define ERROR(...)	SDL_LogError(SDL_LOG_CATEGORY_ERROR, __VA_ARGS__);
#define DEBUG(...)	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__);
#define INFO(...)	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__);

class Ecore
{
public:
	Ecore();
	~Ecore();

	//Starts up SDL and creates window
	bool init();

	void Start();
	void Render(Uint32 currentTime);
	void Update(Uint32 currentTime);

	SDL_Renderer* getRenderer();
	TTF_Font* getFont();

private:
	//Loads media
	bool loadMedia();

	//Frees media and shuts down SDL
	void close();

	inline bool handleEvent(SDL_Event *e);

	//The window we'll be rendering to
	SDL_Window* gWindow;

	//The window renderer
	SDL_Renderer* gRenderer;

	ETexture boxTexture;
	ETexture textTexture;

	TTF_Font *gFont;
};

