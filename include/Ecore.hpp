#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

#include "util/PythonScript.hpp"

class EScreenManager;
class EResourceManager;

class Ecore
{
public:
	~Ecore();
	static Ecore*	getInstance();
	static void		releaseInstance();

	/* SDL variables */
	SDL_Renderer* getRenderer();
	TTF_Font* getFont();
	SDL_Window* getWindow();

	/* Starts up SDL and creates window */
	bool init();

	void Start();
	void Render(Uint32 currentTime, Uint32 accumulator);
	void Update(Uint32 currentTime, Uint32 accumulator = 0);

	/* Provide current FPS */
	double GetFPS();

	EResourceManager& getResourceManager();
	EScreenManager& getScreenManager();

	/* Utility functions */
	static const char* getBasePath();
	static const char* getStorePath();
	static std::string makeBasePath(std::string child_dir);
	static std::string getResourcePath();
	static std::string getResourcePath(std::string file_name);
	static std::string getPlatform();
	static bool checkPlatform(std::string);

private:
	Ecore();

	static Ecore*	instance;

	/* Loads game resource */
	bool loadResources();

	//Frees media and shuts down SDL
	void deinit();

	inline bool handleEvent(SDL_Event *e);

	//The window we'll be rendering to
	SDL_Window* gWindow;

	//The window renderer
	SDL_Renderer* gRenderer;

	TTF_Font *gFont;

	double d_fps;

	/* Manager classes:
	 * Handle these as a pointer type for obvious resource deallocation.
	 */
	EResourceManager* resManager;
	EScreenManager* screenManager;

	static std::string getParentPath(std::string path, std::string::size_type level);
};
