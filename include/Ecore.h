#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "EScreenManager.h"
#include "drawable/ETexture.h"

#define ERROR(...)	SDL_LogError(SDL_LOG_CATEGORY_ERROR, __VA_ARGS__);
#define DEBUG(...)	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__);
#define INFO(...)	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__);

class EScreenManager;

class Ecore
{
public:
	~Ecore();
	static Ecore*	getInstance();

	//Starts up SDL and creates window
	bool init();

	void Start();
	void Render(Uint32 currentTime, Uint32 accumulator);
	void Update(Uint32 currentTime, Uint32 accumulator = 0);

	SDL_Renderer* getRenderer();
	TTF_Font* getFont();

	EScreenManager*		screenManager;

private:
	Ecore();

	static Ecore*	instance;
	//Loads media
	bool loadMedia();

	//Frees media and shuts down SDL
	void close();

	inline bool handleEvent(SDL_Event *e);

	//The window we'll be rendering to
	SDL_Window* gWindow;

	//The window renderer
	SDL_Renderer* gRenderer;

	TTF_Font *gFont;
};
