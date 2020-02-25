#pragma once

#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "util/PythonScript.hpp"
#include "util/SDLWrap.hpp"

namespace story {
namespace Resource {
  class EResourceManager;
}
}

namespace story {
namespace Core {

class ScreenManager;

enum CustomEvent {
  CUSTOM_EVENT_SCENE_CHANGE = 0,
  CUSTOM_EVENT_MAX
};

class Ecore
{
public:
  Ecore();
  ~Ecore();
  static Ecore* getInstance();
  static void   releaseInstance();

  /* SDL variables */
  SDL_Renderer* getRenderer();
  static std::shared_ptr<SDLFontWrap> loadFont(std::string family, int size);
  SDL_Window* getWindow();

  /* Starts up SDL and creates window */
  bool init(void* hwnd);
  void* GetHwnd();

  void Start();
  void Render(Uint32 currentTime, Uint32 accumulator);
  void Update(Uint32 currentTime, Uint32 accumulator = 0);

  /* Provide current FPS */
  double GetFPS();

  story::Resource::EResourceManager& getResourceManager();
  story::Core::ScreenManager& getScreenManager();

  /* Utility functions */
  static const char* getBasePath();
  static const char* getStorePath();
  static std::string makeBasePath(std::string child_dir);
  static std::string getResourcePath();
  static std::string getResourcePath(std::string file_name);
  static std::string getPlatform();
  static bool checkPlatform(std::string);
  static void checkPathContents();

  static int getScreenWidth();
  static int getScreenHeight();
  static uint32_t getAppTicks();
  static bool isHighDPI();
  static float getDisplayScale();

  /* Related with Application life cycle.
   * These function should be executed on application code directly.
   * (Not through on SDL_Event !) */
  static void startAppPauseState();
  static void endAppPauseState();

  /* Request to create a event */
  static int getEventID(CustomEvent event);
  static void requestSceneChange(std::string scene_name);
  void handleSceneChange(std::string scene_name);

private:
  static Ecore* instance;
  static int custom_event_id[CUSTOM_EVENT_MAX];

  /* Frees media and shuts down SDL */
  void deinit();

  bool initialized = false;

  /* Loads game resource */
  bool loadProject();

  inline bool handleEvent(SDL_Event *e);

  //The window we'll be rendering to
  SDL_Window* gWindow;

  //The window renderer
  SDL_Renderer* gRenderer;

  double d_fps;

  /* DPI-aware */
  static bool is_retina_display;
  static float display_scale;

  /* Total amount of paused ticks. */
  static uint32_t in_paused_ticks;

  /* The tick of starting pause */
  static uint32_t start_pause_tick;

  /* Manager classes:
   * Handle these as a pointer type for obvious resource deallocation.
   */
  story::Resource::EResourceManager* resManager;
  story::Core::ScreenManager* screenManager;

  static std::string getParentPath(std::string path, std::string::size_type level);
};

} /* namespace Core */
} /* namespace story */
