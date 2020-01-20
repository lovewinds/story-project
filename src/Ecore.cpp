#include <stdarg.h>
#include <dirent.h>

#include <SDL_syswm.h>
#include <SDL_keyboard.h>

#include "Ecore.hpp"
#include "EScreenManager.hpp"
#include "texture/EDrawable.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EResourceFactory.hpp"

#include "util/LogHelper.hpp"
#include "util/JsonHelper.hpp"
#include "util/XmlHelper.hpp"

/* Screen dimension constants */
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

Ecore* Ecore::instance = NULL;
int Ecore::custom_event_id[CUSTOM_EVENT_MAX];
uint32_t Ecore::in_paused_ticks = 0;
uint32_t Ecore::start_pause_tick = 0;

bool Ecore::is_retina_display = false;
float Ecore::display_scale = 1.0f;

Ecore::Ecore() :
gWindow(NULL),
gRenderer(NULL)
{
#ifndef USE_SDL_LOG
	Log::init();
#endif
	PythonScript::preparePath(getResourcePath());
	/* Teporary relative path
	 * TODO: Directory policy should be decided */
	/* build/lib/python */
#if defined(PLATFORM_WINDOWS)
	PythonScript::preparePath(makeBasePath(".."));
	PythonScript::preparePath(makeBasePath("..\\lib"));
	PythonScript::preparePath(makeBasePath("..\\lib\\python"));
	PythonScript::preparePath(makeBasePath("python37_modules_Lib.zip"));
	PythonScript::preparePath(makeBasePath("python37_modules_Tools.zip"));
#elif defined(PLATFORM_IOS)
	PythonScript::preparePath(makeBasePath("python37.zip"));
	PythonScript::preparePath(makeBasePath("python"));
#else
	/* macOS */
	/* Linux */
	PythonScript::preparePath(makeBasePath("python3.7"));
#endif
	PythonScript::initialize();

	Ecore::instance = this;

	for(int i = 0; i < CUSTOM_EVENT_MAX; i++) {
		custom_event_id[i] = SDL_RegisterEvents(1);
	}

	srand(time(NULL));
}

Ecore::~Ecore()
{
	deinit();
	PythonScript::finalize();
#ifndef USE_SDL_LOG
	Log::deinit();
#endif
}

Ecore*	Ecore::getInstance()
{
	if (!instance) {
		instance = new Ecore();
	}

	return instance;
}

void Ecore::releaseInstance()
{
	if (instance) {
		delete instance;
		instance = NULL;
	}
}

inline bool Ecore::handleEvent(SDL_Event *e)
{
	bool proceed = true;

	switch(e->type) {
	case SDL_QUIT:
		//User requests quit
		proceed = false;
	break;
	case SDL_WINDOWEVENT:
		LOG_DBG("SDL_WINDOWEVENT : [%X]", e->window.event);
		switch (e->window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			proceed = false;
			break;
		case SDL_WINDOWEVENT_RESIZED: {
			LOG_DBG("SDL_WINDOWEVENT_RESIZED");
			int dr_width = 0;
			int dr_height = 0;
			int win_width = 0;
			int win_height = 0;
			SDL_GL_GetDrawableSize(gWindow, &dr_width, &dr_height);
			SDL_GetWindowSize(gWindow, &win_width, &win_height);

			LOG_INFO("Drawable size [%4d x %4d]", dr_width, dr_height);
			LOG_INFO("Window   size [%4d x %4d]", win_width, win_height);
			if (dr_width != win_width && dr_height != win_height) {
				Ecore::is_retina_display = true;
				Ecore::display_scale = 1.0f;
			}
			else {
				if (Ecore::is_retina_display)
					Ecore::display_scale = 1.0f;
				Ecore::is_retina_display = false;
			}
			break;
		}
#ifdef PLATFORM_WINDOWS
		case SDL_WINDOWEVENT_MOVED: {
			/* Per-monitor DPI awareness */
			int totalDisplayNum = SDL_GetNumVideoDisplays();
			int currentDisplayIdx = SDL_GetWindowDisplayIndex(gWindow);
			float currentDPI = 0.0f;
			float scale = 1.0f;
			bool isDPIChanged = false;

			SDL_GetDisplayDPI(currentDisplayIdx, &currentDPI, NULL, NULL);
			scale = (currentDPI / 96.0f);
			if (scale != Ecore::display_scale) {
				isDPIChanged = true;
				Ecore::display_scale = scale;
			}

			LOG_DBG("Current display [%d / %d]", currentDisplayIdx + 1, totalDisplayNum);
			LOG_DBG("   DPI  : %f", currentDPI);
			LOG_DBG("   Scale: %f", Ecore::display_scale);

			if (false == Ecore::is_retina_display && isDPIChanged) {
				LOG_DBG("DPI changed. Resize window");
				SDL_SetWindowSize(gWindow,
					(int)(SCREEN_WIDTH * Ecore::display_scale),
					(int)(SCREEN_HEIGHT * Ecore::display_scale));
			}
			break;
		}
#endif
		default: break;
		}
	break;
	case SDL_APP_WILLENTERBACKGROUND:
		LOG_DBG("SDL_APP_WILLENTERBACKGROUND");
	break;
	case SDL_APP_DIDENTERBACKGROUND:
		LOG_DBG("SDL_APP_DIDENTERBACKGROUND");
		startAppPauseState();
	break;
	case SDL_APP_WILLENTERFOREGROUND:
		LOG_DBG("SDL_APP_WILLENTERFOREGROUND");
		endAppPauseState();
	break;
	case SDL_APP_DIDENTERFOREGROUND:
		LOG_DBG("SDL_APP_DIDENTERFOREGROUND");
	break;
	case SDL_USEREVENT:
		LOG_DBG("Got USER_EVENT !!");
		if (e->user.code == custom_event_id[CUSTOM_EVENT_SCENE_CHANGE]) {
			/* Handle scene change event */
			char *p_name = (char *)e->user.data1;
			std::string scene_name(p_name);

			LOG_DBG("Scene change event [%s]", p_name);
			handleSceneChange(scene_name);

			/* Dynamic allocated string should be freed */
			SDL_free(p_name);
			e->user.data1 = nullptr;
		}
	break;
	case SDL_MOUSEMOTION: {
		if (Ecore::isHighDPI() && (false == Ecore::is_retina_display)) {
			e->motion.x = e->motion.x / Ecore::display_scale;
			e->motion.y = e->motion.y / Ecore::display_scale;
			e->motion.xrel = e->motion.xrel / Ecore::display_scale;
			e->motion.yrel = e->motion.yrel / Ecore::display_scale;
		}
		break;
	}
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEBUTTONDOWN: {
		if (Ecore::isHighDPI() && (false == Ecore::is_retina_display)) {
			e->button.x = e->button.x / Ecore::display_scale;
			e->button.y = e->button.y / Ecore::display_scale;
		}
		break;
	}
	default: break;
	}

	return proceed;
}

void Ecore::Start()
{
	/* Start up SDL and create window */
	if (initialized == false) {
		if (init(nullptr) == false) {
			LOG_ERR("Failed to initialize!");
			return;
		}
		initialized = true;
	}

	screenManager = new EScreenManager();
	resManager = new EResourceManager();
	resFactory = new EResourceFactory();

	/* Load game resources */
	if (loadResources() == false)
	{
		LOG_ERR("Failed to load media!");
	}
	else
	{
		/* Main loop flag */
		bool quit = false;

		/* Event handler */
		SDL_Event e;

		/* Flip type */
		SDL_RendererFlip flipType = SDL_FLIP_NONE;

		/* Time management */
		const Uint32 MAX_FRAMESKIP = 5;			/* Maximun count of skipping frame rendering */
		const Uint32 UPDATE_TICKS_PER_SECOND = 25;	/* Tick time for each update function. it affects game speed */
		const Uint32 RENDER_TICKS_PER_SECOND = 59;	/* Tick time for each render function. it affects FPS */
		Uint32 t = 0;
		const Uint32 SKIP_UPDATE_TICKS = 1000 / UPDATE_TICKS_PER_SECOND;
		const Uint32 SKIP_RENDER_TICKS = 1000 / RENDER_TICKS_PER_SECOND;

		Uint32 accumulator = 0;
		Uint32 prevTime = getAppTicks();
		Uint32 currentTime = prevTime;
		Uint32 nextUpdateTick = prevTime;
		Uint32 nextRenderTick = prevTime;
		Uint32 frameTime = 0;
		Uint32 prevCalculated = prevTime;
		int loops = 0;
		int updated = 0;
		int rendered = 0;
		double alpha = 0.0;

		LOG_INFO("Start !!");
		/* While application is running */
		while (!quit)
		{
			/* Check Event */
			while (SDL_PollEvent(&e) != 0) {
				if (handleEvent(&e) == false) {
					quit = true;
				}

				screenManager->handleEvent(e);
			}

			currentTime = getAppTicks();
			frameTime = currentTime - prevTime;	/* elapsed time from last frame rendering */
			//accumulator += frameTime;

			//SDL_Log("Prev: %f / Curr: %f / frameTime: %f", prevTime, currentTime, frameTime);
			//INFO("Prev: %d / Curr: %d / frameTime: %d", prevTime, currentTime, frameTime);

			//frameTime = SDL_min(frameTime, maxFrameTime);
			//while (frameTime > 0.0)	{
			//while (accumulator > 0.0)	{
			loops = 0;
			accumulator = 0;
			while (currentTime > nextUpdateTick && loops < MAX_FRAMESKIP)
			{
				//const Uint32 deltaTime = SDL_min(accumulator, dt);

				//frameTime -= deltaTime;
				//accumulator -= deltaTime;
				//accumulator -= frameTime;

				// minus value of Uint32
				//accumulator -= SKIP_UPDATE_TICKS;
				//t += deltaTime;

				/* Update Sprites */
				//Update(currentTime, deltaTime);
				//alpha = accumulator / SKIP_UPDATE_TICKS;
				Update(currentTime);
				accumulator += currentTime - nextUpdateTick;

				/* Set next tick time */
				nextUpdateTick += SKIP_UPDATE_TICKS;

				loops++;

				//SDL_Log("t: %f / frameTime: %f / dt: %f", t, frameTime, dt);
#if 0
				INFO("Curr [%d] / next [%d] / diff [%d] / ACC [%d]",
					currentTime, nextUpdateTick, nextUpdateTick - currentTime, accumulator);
#endif

				currentTime = getAppTicks();
			}
			updated += loops;

			/* Calculate alpha accumulator */
			//alpha = double(currentTime + SKIP_UPDATE_TICKS - nextUpdateTick) / double(SKIP_UPDATE_TICKS);
			alpha = double(accumulator) / double(SKIP_UPDATE_TICKS);

			//SDL_Log("Render !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			//INFO("Render !! / Updated : [%d] times", loops);
#if 1
			/* Limit frames */
			//currentTime = getAppTicks();
			//if (currentTime - prevTime <= 1000 / 60) {
			if (currentTime - prevCalculated > 1000) {
				//SDL_Delay(1000 / 60);
				//LOG_INFO("[%d] : Updated: [%d] / Rendered [%d]", currentTime, updated, rendered);
				updated = 0;
				rendered = 0;
				//continue;
				prevCalculated = currentTime;
			}
#endif
			prevTime = currentTime;

			currentTime = getAppTicks();
			if (currentTime <= nextRenderTick) {
				//nextRenderTick += SKIP_RENDER_TICKS;
				continue;
			}
			nextRenderTick += SKIP_RENDER_TICKS;
			//INFO("currentTime: [%d] / nextRenderTick [%d]", currentTime, nextRenderTick);

			/* Render screen */
			Render(currentTime, accumulator);

			rendered++;

			/* Put a delay to lower CPU usage */
			SDL_Delay(1);
		}
	}

	/* Free resources and close SDL */
	//deinit();
}

void Ecore::Render(Uint32 currentTime, Uint32 accumulator)
{
	static Uint32 prevTime = 0;
	static unsigned int drawed_frames = 0;

	char str[256] = { 0, };

	/* Update latest position */
	Update(currentTime, accumulator);

	/* Increase frame counter */
	drawed_frames++;

	/* Calculate FPS */
	currentTime = getAppTicks();
	if (currentTime - prevTime >= 1000) {
		d_fps = (double)drawed_frames / (double)(currentTime - prevTime) * 1000.0;
		prevTime = currentTime;
		drawed_frames = 1;
	}

	/* Clear screen */
	//SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_SetRenderDrawColor(gRenderer, 0x1B, 0x40, 0x5E, 0xFF);
	SDL_SetRenderDrawColor(gRenderer, 0x10, 0x30, 0x4F, 0xC0);
	SDL_RenderClear(gRenderer);

	/* Render Screen */
	screenManager->render();

	// Render Color
	//colorTexture.draw();
	//colorTexture.render_resize(64, 64, 0, drawed_frames / 10, 0, SDL_FLIP_NONE);

	/* Update SDL Screen */
	SDL_RenderPresent(gRenderer);
}

void Ecore::Update(Uint32 currentTime, Uint32 accumulator)
{
	screenManager->update(currentTime, accumulator);
}

double Ecore::GetFPS()
{
	return d_fps;
}

EResourceManager& Ecore::getResourceManager()
{
	return *resManager;
}

EResourceFactory& Ecore::getResourceFactory()
{
	return *resFactory;
}

EScreenManager& Ecore::getScreenManager()
{
	return *screenManager;
}

void* Ecore::GetHwnd()
{
#ifdef PLATFORM_WINDOWS
	struct SDL_SysWMinfo wmInfo;

	/* initialize info structure with SDL version info */
	SDL_VERSION(&wmInfo.version);

	if (gWindow) {
		SDL_GetWindowWMInfo(gWindow, &wmInfo);

		return wmInfo.info.win.window;
	}
#endif
	return NULL;
}

bool Ecore::init(void* hwnd)
{
	//createJson();
	//readJson();
	int totalDisplayNum = 0;
	int currentDisplayIdx = 0;
	float currentDPI = 0.0f;

	LOG_DBG("Base path: %s", getBasePath());
	LOG_DBG("Pref path: %s", getStorePath());

	/* Initialization flag */
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_ERR("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	/* Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
	{
		LOG_ERR("Warning: Linear texture filtering not enabled!");
	}

	/* Create window */
	LOG_ERR("HWND : [%p]", hwnd);
	if (hwnd != NULL) {
#if 1
		//SDL_Window* tmp = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_BORDERLESS);
		//char sBuf[64];
		//if (tmp != 0) {
			//SDL_snprintf(sBuf, 64, "%p", tmp);

			/* Set SDL hint to display external window properly */
			//if (SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, sBuf) == false) {
			//	LOG_ERR("Warning: Hint for external window failed !");
			//}
			gWindow = SDL_CreateWindowFrom(hwnd);
			//SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, nullptr);
			//SDL_DestroyWindow(tmp);
		//}
#else
		gWindow = SDL_CreateWindow("Story",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
#endif
	}
	else {
		Uint32 flags = SDL_WINDOW_SHOWN;

		flags |= SDL_WINDOW_ALLOW_HIGHDPI;
		gWindow = SDL_CreateWindow("Story",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, flags);
	}
	if (gWindow == NULL) {
		LOG_ERR("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	/* Create vsynced renderer for window */
	/* VSync: SDL_RENDERER_PRESENTVSYNC */
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		LOG_ERR("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		int dr_width = 0, dr_height = 0;
		int win_width = 0, win_height = 0;
		SDL_GL_GetDrawableSize(gWindow, &dr_width, &dr_height);
		SDL_GetWindowSize(gWindow, &win_width, &win_height);

		LOG_INFO("Drawable size [%4d x %4d]", dr_width, dr_height);
		LOG_INFO("Window   size [%4d x %4d]", win_width, win_height);
		if (dr_width != win_width && dr_height != win_height) {
			Ecore::is_retina_display = true;
			Ecore::display_scale = dr_width / win_width;
		}

		/* Per-monitor DPI awareness (Windows) */
		totalDisplayNum = SDL_GetNumVideoDisplays();
		currentDisplayIdx = SDL_GetWindowDisplayIndex(gWindow);
		SDL_GetDisplayDPI(currentDisplayIdx, &currentDPI, NULL, NULL);
		if (false == Ecore::is_retina_display)
			Ecore::display_scale = (currentDPI / 96.0f);

		LOG_INFO("Current display [%d / %d]", currentDisplayIdx+1, totalDisplayNum);
		LOG_INFO("   DPI  : %f", currentDPI);
		LOG_INFO("   Scale: %f", Ecore::display_scale);
		
		if (false == Ecore::is_retina_display) {
			SDL_SetWindowSize(gWindow,
				(int)(SCREEN_WIDTH * Ecore::display_scale),
				(int)(SCREEN_HEIGHT * Ecore::display_scale));
		}

#ifdef PLATFORM_IOS
		/* iCade Support */
		SDL_StartTextInput();
		SDL_EventState(SDL_TEXTINPUT, SDL_ENABLE);
		if (SDL_FALSE == SDL_IsScreenKeyboardShown(Ecore::getInstance()->getWindow())) {
				LOG_ERR("There is no screen keyboard !!!!!!!!!!");
				/* Don't stop text input state for hardware keyboard */
		} else {
			LOG_DBG("Screen keyboard is present !!!!!!!!!!!");
			/* But not used currently */
			SDL_StopTextInput();
		}
#else
		/* Disable text input */
		SDL_StopTextInput();
#endif

		/* Initialize renderer color */
		//SDL_SetRenderDrawColor(gRenderer, 0x1B, 0x40, 0x5E, 0xFF);
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x33, 0x99, 0xFF);

		/* Initialize PNG loading */
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			LOG_ERR("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			success = false;
		}

		/* Initialize SDL_ttf */
		if (TTF_Init() == -1)
		{
			LOG_ERR("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}

	if (success)
		initialized = true;
#ifdef PLATFORM_WINDOWS
	/* TODO: Should be check
		this app can be executed with internal window state on other platforms */
	if (success && hwnd != NULL)
		SetParent((HWND)GetHwnd(), (HWND)hwnd);
#endif

	return success;
}

std::shared_ptr<SDLFontWrap> Ecore::loadFont(std::string family, int size)
{
	std::shared_ptr<SDLFontWrap> font = nullptr;
	char font_path[256] = { 0, };
	TTF_Font *pFont = NULL;

	/* Load Font */
	SDL_snprintf(font_path, 256, "../res/%s.ttf", family.c_str());
	pFont = TTF_OpenFont(font_path, size);
	if (pFont == NULL)
	{
		/* Android can handle under /assets directory */
		SDL_snprintf(font_path, 256, "%s.ttf", family.c_str());
		pFont = TTF_OpenFont(font_path, size);
		if (pFont == NULL) {
			LOG_ERR("Failed to load font[%s]!", family.c_str());
			LOG_ERR("      SDL_ttf Error: %s", TTF_GetError());
			return nullptr;
		}
	}
	font = std::shared_ptr<SDLFontWrap>(new SDLFontWrap(pFont, family, size));

	return font;
}

bool Ecore::loadResources()
{
	/* Loading success flag */
	bool success = true;

	/* Load resources
	 * TODO: This logic should 'prepare' to allocate all resources.
	 */
	std::string res_xml("sample_scene.xml");
	if (Ecore::checkPlatform("Linux"))
		res_xml = "sample_scene2.xml";
	success = resManager->loadResources(res_xml);
	if (!success) {
		LOG_ERR("Failed to load resources!");
		return false;
	}
	//success = screenManager->loadResources(std::string("sample_scene.xml"));

	/* Create debug scene
	 *   It displays some helpful variables on transparent screen.
	 *     e.g. FPS
	 */
	screenManager->initDebugScene();

	/* Play default scene
	 *   This logic allocates all required resources into memory
	 *   and launch specific screen.
	 */
	std::string default_scene("main");
	success = screenManager->playScene(default_scene);
	if (!success) {
		LOG_ERR("Failed to play scene [%s] !", default_scene.c_str());
		return false;
	}

	return success;
}

void Ecore::deinit()
{
	LOG_DBG("Deinitialize Ecore");

	/* Release resources */
	delete screenManager;
	screenManager = NULL;
	delete resManager;
	resManager = NULL;

	/* Destroy window */
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	/* Quit SDL subsystems */
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Renderer* Ecore::getRenderer()
{
	return gRenderer;
}

SDL_Window* Ecore::getWindow()
{
	return gWindow;
}

int Ecore::getScreenWidth()
{
	int w;
	int h;
	SDL_GetWindowSize(Ecore::getInstance()->getWindow(), &w, &h);

	if (!is_retina_display)
		w = (int)(w / display_scale);

	return w;
}

int Ecore::getScreenHeight()
{
	int w;
	int h;
	SDL_GetWindowSize(Ecore::getInstance()->getWindow(), &w, &h);

	if (!is_retina_display)
		h = (int)(h / display_scale);

	return h;
}

bool Ecore::isHighDPI()
{
	bool result = false;

	if (is_retina_display)
		result = true;
	else if (display_scale > 1.0f)
		result = true;

	return result;
}

float Ecore::getDisplayScale()
{
	return display_scale;
}

uint32_t Ecore::getAppTicks()
{
	return SDL_GetTicks() - in_paused_ticks;
}

void Ecore::startAppPauseState()
{
	start_pause_tick = SDL_GetTicks();
}

void Ecore::endAppPauseState()
{
	uint32_t in_pause = SDL_GetTicks() - start_pause_tick;
	in_paused_ticks += in_pause;
}

const char* Ecore::getBasePath()
{
	static const char* path = SDL_GetBasePath();
	return path;
}

const char* Ecore::getStorePath()
{
	static const char* path = SDL_GetPrefPath("ariens", "story-project");
	return path;
}

std::string Ecore::getParentPath(std::string path, std::string::size_type level)
{
	std::vector<std::string> tokens;
	static std::string delim;
	std::string result;

	if (Ecore::checkPlatform("Windows"))
		delim = "\\";
	else {
		delim = "/";
		result.append(delim);
	}
	std::string::size_type lastPos = path.find_first_not_of(delim, 0);
	std::string::size_type pos = path.find_first_of(delim, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		std::string s(path.substr(lastPos, pos-lastPos));
		tokens.push_back(s);
		lastPos = path.find_first_not_of(delim, pos);
		pos = path.find_first_of(delim, lastPos);
	}

	for (std::string::size_type i = 0 ; i < tokens.size() - level ; i++)
	{
		if (i != 0)
			result.append(delim);
		result.append(tokens[i]);
	}
	result.append(delim);

	return result;
}

std::string Ecore::getResourcePath()
{
	std::string path(SDL_GetBasePath());
	static std::string platform(SDL_GetPlatform());

	/* TODO:
	 *  below path is not considering the installation path.
	 *  The path should consider installation.
	 */
	if ("Windows" == platform) {
		path = getParentPath(path, 2);
		path.append("res");
		//LOG_INFO("Windows platform: [%s]", path.c_str());
	} else if ("Linux" == platform) {
		path = getParentPath(path, 1);
		path.append("res");
		//LOG_INFO("Linux platform: [%s]", path.c_str());
	} else if ("Mac OS X" == platform) {
		path = getParentPath(path, 1);
		path.append("res");
		//LOG_INFO("Mac OS X platform: [%s]", path.c_str());
	}

	return path;
}

std::string Ecore::getResourcePath(std::string file_name)
{
	if (file_name.empty()) {
		LOG_ERR("Resource file is not provided !");
		return std::string();
	}
	std::string path(SDL_GetBasePath());
	static std::string platform(SDL_GetPlatform());

	/* TODO:
	 *  below path is not considering the installation path.
	 *  The path should consider installation.
	 */
	if ("Windows" == platform) {
		path = getParentPath(path, 2);
		path.append("res\\");
		//LOG_INFO("Windows platform: [%s]", path.c_str());
	} else if ("Linux" == platform) {
		path = getParentPath(path, 1);
		path.append("res/");
		//LOG_INFO("Linux platform: [%s]", path.c_str());
	} else if ("Mac OS X" == platform) {
		path = getParentPath(path, 1);
		path.append("res/");
		//LOG_INFO("Mac OS X platform: [%s]", path.c_str());
	}
	path.append(file_name);

	return path;
}

std::string Ecore::makeBasePath(std::string child_dir)
{
	std::string path(SDL_GetBasePath());

	if (child_dir.compare(".") != 0)
		path.append(child_dir);

	return path;
}

void Ecore::checkPathContents()
{
	DIR            *dir_info;
	struct dirent  *dir_entry;

	dir_info = opendir(getBasePath());
	if (NULL != dir_info)
	{
        LOG_INFO("%s", getBasePath());
		while((dir_entry = readdir(dir_info)))
		{
			LOG_INFO( "  %s", dir_entry->d_name);
		}
		closedir(dir_info);
	}
    
    LOG_INFO(" ");
    LOG_INFO(" ");
    std::string ppath = makeBasePath("python");
    dir_info = opendir(ppath.c_str());
    if (NULL != dir_info)
    {
        LOG_INFO("%s", getBasePath());
        while((dir_entry = readdir(dir_info)))
        {
            LOG_INFO( "  %s", dir_entry->d_name);
        }
        closedir(dir_info);
    }   
}

std::string Ecore::getPlatform()
{
	std::string platform(SDL_GetPlatform());
	return platform;
}

bool Ecore::checkPlatform(std::string platform)
{
	static std::string p(SDL_GetPlatform());
	return (p == platform);
}

int Ecore::getEventID(CustomEvent event)
{
	return custom_event_id[event];
}

void Ecore::requestSceneChange(std::string scene_name)
{
	if (scene_name.empty()) {
		LOG_ERR("Invalid scene name");
		return;
	}

	if (custom_event_id[CUSTOM_EVENT_SCENE_CHANGE] != ((Uint32)-1)) {
		SDL_Event event;
		int ret = 0;

		SDL_memset(&event, 0, sizeof(event)); /* or SDL_zero(event) */
		event.type = SDL_USEREVENT;
		event.user.code = custom_event_id[CUSTOM_EVENT_SCENE_CHANGE];
		event.user.data1 = SDL_strdup(scene_name.c_str());
		event.user.data2 = 0;

		LOG_DBG("USER_EVENT created [CUSTOM_EVENT_SCENE_CHANGE(%d)] [%s]",
			custom_event_id[CUSTOM_EVENT_SCENE_CHANGE],
			(char *)event.user.data1);

		ret = SDL_PushEvent(&event);
		if (ret <= 0) {
			LOG_ERR("Failed to push event ! [%d]", ret);
		}
	} else {
		LOG_ERR("CRITICAL: event_id was not allocated properly !");
	}
}

void Ecore::handleSceneChange(std::string scene_name)
{
	bool success = screenManager->playScene(scene_name);
	if (!success) {
		LOG_ERR("Failed to play scene [%s] !", scene_name.c_str());
	}
}
