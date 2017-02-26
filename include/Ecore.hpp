#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

#include "util/PythonScript.hpp"

class EScreenManager;
class EResourceManager;
class EResourceFactory;

class Ecore
{
public:
	Ecore();
	~Ecore();
	static Ecore*	getInstance();
	static void		releaseInstance();

	/* SDL variables */
	SDL_Renderer* getRenderer();
	TTF_Font* getFont();
	SDL_Window* getWindow();

	/* Starts up SDL and creates window */
	bool init(void* hwnd);
	void* GetHwnd();

	void Start();
	void Render(Uint32 currentTime, Uint32 accumulator);
	void Update(Uint32 currentTime, Uint32 accumulator = 0);

	/* Provide current FPS */
	double GetFPS();

	EResourceManager& getResourceManager();
	EResourceFactory& getResourceFactory();
	EScreenManager& getScreenManager();

	/* Utility functions */
	static const char* getBasePath();
	static const char* getStorePath();
	static std::string makeBasePath(std::string child_dir);
	static std::string getResourcePath();
	static std::string getResourcePath(std::string file_name);
	static std::string getPlatform();
	static bool checkPlatform(std::string);

	static int getScreenWidth();
	static int getScreenHeight();

private:
	

	static Ecore*	instance;

	/* Frees media and shuts down SDL */
	void deinit();

	bool initialized = false;

	/* Loads game resource */
	bool loadResources();

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
	EResourceFactory* resFactory;
	EScreenManager* screenManager;

	static std::string getParentPath(std::string path, std::string::size_type level);
};
