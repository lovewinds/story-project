#include "ScreenManager.hpp"

#include "core/Ecore.hpp"
#include "core/SceneManager.hpp"
#include "util/LogHelper.hpp"

namespace story {
namespace Core {

ScreenManager::ScreenManager()
{
  sceneManager = new SceneManager();
}

ScreenManager::~ScreenManager()
{
  if (nullptr != sceneManager) {
    delete sceneManager;
    sceneManager = nullptr;
  }
}

void ScreenManager::initDebugScene()
{
  sceneManager->initDebugScene();
}

bool ScreenManager::playScene(std::string scene_name)
{
  bool success = true;

  /* Scene handler will allocate textures with scene instance */
  success = sceneManager->playScene(scene_name);

#if 0
  std::string platform = Ecore::getInstance()->getPlatform();
  if (Ecore::checkPlatform("Linux")) {
    LOG_INFO("Platform: [%s]", platform.c_str());
    success = false;
  }
#endif

  return success;
}

void ScreenManager::render()
{
  /* Render through sceneManager */
  sceneManager->render();
}

void ScreenManager::update(Uint32 currentTime, Uint32 accumulator)
{
  /* Update through sceneManager */
  sceneManager->update(currentTime, accumulator);
}

void ScreenManager::handleEvent(SDL_Event e)
{
  if (e.type == SDL_MOUSEBUTTONDOWN
    || e.type == SDL_KEYDOWN || e.type == SDL_TEXTINPUT /* Key pressed */
    || e.type == SDL_KEYUP  /* Key released */
    || e.type == SDL_FINGERDOWN
    || e.type == SDL_FINGERUP
    || e.type == SDL_FINGERMOTION)
  {
    //LOG_INFO("[TEST] Event : [%x]", e.type);
    /* Propagate event into scene */
    sceneManager->handleEvent(e);
  }
#if 0
  /* Handle Keyboard state */
  const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
  if (currentKeyStates[SDL_SCANCODE_W]) {
    LOG_INFO("    Key state [W] / e.type : [%x]", e.type);
    sceneManager->temp_moveBackGround(0.0, -1.0);
  }
  if (currentKeyStates[SDL_SCANCODE_A]) {
    LOG_INFO("    Key state [A] / e.type : [%x]", e.type);
    sceneManager->temp_moveBackGround(-1.0, 0.0);
  }
  if (currentKeyStates[SDL_SCANCODE_S]) {
    LOG_INFO("    Key state [S] / e.type : [%x]", e.type);
    sceneManager->temp_moveBackGround(0.0, 1.0);
  }
  if (currentKeyStates[SDL_SCANCODE_D]) {
    LOG_INFO("    Key state [D] / e.type : [%x]", e.type);
    sceneManager->temp_moveBackGround(1.0, 0.0);
  }
#endif
}

} /* namespace Core */
} /* namespace story */
