#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "EScreenManager.h"
#include "texture/ETexture.h"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.h"

#if 0
#define ERROR(...)	SDL_LogError(SDL_LOG_CATEGORY_ERROR, __VA_ARGS__);
#define DEBUG(...)	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__);
#define INFO(...)	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__);
#endif

class EScreenManager;

class Ecore
{
public:
	~Ecore();
	static Ecore*	getInstance();

	/* SDL variables */
	SDL_Renderer* getRenderer();
	TTF_Font* getFont();
	SDL_Window* getWindow();

	EScreenManager*		screenManager;

	/* Starts up SDL and creates window */
	bool init();

	void Start();
	void Render(Uint32 currentTime, Uint32 accumulator);
	void Update(Uint32 currentTime, Uint32 accumulator = 0);

	/* Provide current FPS */
	double GetFPS();
	const char* getBasePath();
	const char* getStorePath();

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

	EResourceManager resManager;

	double d_fps;
};
