#pragma once

#include <string>
#include <SDL.h>

namespace story {
namespace Core {

class SceneManager;

class ScreenManager
{
public:
  ScreenManager();
  ~ScreenManager();

  /* Prohibit copy constructor */
  ScreenManager(ScreenManager& self) = delete;

  bool playScene(std::string scene_name);
  void initDebugScene();

  /* Primitive functions */
  void render();
  void update(Uint32 currentTime, Uint32 accumulator);
  void handleEvent(SDL_Event e);

private:
  SceneManager* sceneManager;
};

} /* namespace Core */
} /* namespace story */
