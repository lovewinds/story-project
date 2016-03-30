#include "EScreenManager.hpp"

#include "texture/ETextureHandler.h"
#include "resource/EResourceManager.hpp"
#include "util/LogHelper.hpp"

EScreenManager::EScreenManager()
{
	//textureHandler = ETextureHandler::getInstance();
	textureHandler = new ETextureHandler();
}

EScreenManager::~EScreenManager()
{
}

bool EScreenManager::loadScene(std::string& scene_name)
{
	bool	success = true;

	EResourceManager& resMgr = Ecore::getInstance()->getResourceManager();
	/* TODO: Load scene instance with resource manager */
	resMgr.allocateScene(scene_name);

	/* TODO: Let texture handler load textures from scene instance */
	// success = textureHandler->loadTextures(scene);
#if 0
	std::string platform = Ecore::getInstance()->getPlatform();
	if (platform.compare("Linux") == 0) {
		LOG_ERR("Platform: [%s]", platform.c_str());
		success = false;
	}
#endif
	success = false;

	return success;
}

bool EScreenManager::createBackgroundImage(std::string& image_path)
{
	return textureHandler->createBackgroundImage(image_path);
}

bool EScreenManager::createSprite(std::string& sprite_path)
{
	return textureHandler->createSprite(sprite_path);
}

void EScreenManager::render()
{
	/* Render through TextureHandler */
	textureHandler->render();
}

void EScreenManager::update(Uint32 currentTime, Uint32 accumulator)
{
	/* Update through TextureHandler */
	textureHandler->update(currentTime, accumulator);
}

void EScreenManager::handleEvent(SDL_Event e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN
		|| e.type == SDL_KEYDOWN || e.type == SDL_TEXTINPUT	/* Key pressed */
		|| e.type == SDL_KEYUP	/* Key released */
		|| e.type == SDL_FINGERDOWN
		|| e.type == SDL_FINGERMOTION)
	{
		LOG_INFO("Event : [%x]", e.type);
		textureHandler->handleEvent(e);
	}
#if 0
	/* Handle Keyboard state */
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_W]) {
		LOG_INFO("    Key state [W] / e.type : [%x]", e.type);
		textureHandler->temp_moveBackGround(0.0, -1.0);
	}
	if (currentKeyStates[SDL_SCANCODE_A]) {
		LOG_INFO("    Key state [A] / e.type : [%x]", e.type);
		textureHandler->temp_moveBackGround(-1.0, 0.0);
	}
	if (currentKeyStates[SDL_SCANCODE_S]) {
		LOG_INFO("    Key state [S] / e.type : [%x]", e.type);
		textureHandler->temp_moveBackGround(0.0, 1.0);
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		LOG_INFO("    Key state [D] / e.type : [%x]", e.type);
		textureHandler->temp_moveBackGround(1.0, 0.0);
	}
#endif
}
