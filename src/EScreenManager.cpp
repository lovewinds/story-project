#include "EScreenManager.hpp"

#include "Ecore.hpp"
#include "ESceneManager.hpp"
#include "resource/EResourceManager.hpp"
#include "util/LogHelper.hpp"
#include "texture/drawable/EDrawable.h"

EScreenManager::EScreenManager()
{
	//sceneManager = ESceneManager::getInstance();
	sceneManager = new ESceneManager();
}

EScreenManager::~EScreenManager()
{
	if (sceneManager) {
		delete sceneManager;
		sceneManager = NULL;
	}
}

bool EScreenManager::playScene(std::string scene_name)
{
	bool success = true;

	/* Scene handler will allocate textures with scene instance */
	success = sceneManager->playScene(scene_name);

	std::string platform = Ecore::getInstance()->getPlatform();
#if 0
	if (Ecore::checkPlatform("Linux")) {
		LOG_INFO("Platform: [%s]", platform.c_str());
		success = false;
	}
#endif

	return success;
}

void EScreenManager::render()
{
	/* Render through sceneManager */
	sceneManager->render();
#if 0
	//std::list<ETexture*>::iterator	iter = textureList.begin();
	std::list<EDrawable*>::iterator	iter = textureList.begin();
	Uint32 current = SDL_GetTicks();

	/* TODO: Handle this texture same with others */
	background->render();

	while (iter != textureList.end()) {
		EDrawable* texture = *iter;
		if (texture != NULL) {
			texture->render();
			//LOG_INFO("List Texture(%p) rendered !", texture);
		} else {
			LOG_INFO("List Texture is NULL. count: %lu", textureList.size());
		}
		iter++;
	}
	textTexture->render();
	sprite->render();
#endif
}

void EScreenManager::update(Uint32 currentTime, Uint32 accumulator)
{
	/* Update through sceneManager */
	sceneManager->update(currentTime, accumulator);
#if 0
	//std::list<ETexture*>::iterator	iter = textureList.begin();
	std::list<EDrawable*>::iterator	iter = textureList.begin();
	while (iter != textureList.end()) {
		EDrawable* texture = *iter;
		if (texture != NULL) {
			//texture->calculate(currentTime, accumulator);
			texture->update(currentTime, accumulator);
		}
		iter++;
	}

	/* TODO: Handle this texture same with others */
	background->update(currentTime, accumulator);

	textTexture->update(currentTime, accumulator);
	sprite->update(currentTime, accumulator);

	/* TEST */
	if (testState & DIR_UP)
		temp_moveCharacter(0.0, -1.0);
	if (testState & DIR_DOWN)
		temp_moveCharacter(0.0, 1.0);
	if (testState & DIR_LEFT)
		temp_moveCharacter(-1.0, 0.0);
	if (testState & DIR_RIGHT)
		temp_moveCharacter(1.0, 0.0);

	if (testBackgroundState & DIR_UP)
		temp_moveBackGround(0.0, -1.0);
	if (testBackgroundState & DIR_DOWN)
		temp_moveBackGround(0.0, 1.0);
	if (testBackgroundState & DIR_LEFT)
		temp_moveBackGround(-1.0, 0.0);
	if (testBackgroundState & DIR_RIGHT)
		temp_moveBackGround(1.0, 0.0);
#endif
}

void EScreenManager::handleEvent(SDL_Event e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN
		|| e.type == SDL_KEYDOWN || e.type == SDL_TEXTINPUT	/* Key pressed */
		|| e.type == SDL_KEYUP	/* Key released */
		|| e.type == SDL_FINGERDOWN
		|| e.type == SDL_FINGERMOTION)
	{
		static Uint32 latestEventTime = 0;
		LOG_INFO("Event : [%x]", e.type);
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
