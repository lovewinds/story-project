#include "ESceneManager.hpp"

#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EResourceFactory.hpp"

ESceneManager::ESceneManager()
{
	currentScene = nullptr;

	/* TEMP: Input state handle */
	testState = DIR_STOP;
	testBackgroundState = DIR_STOP;
	overlayState = false;
}

ESceneManager::~ESceneManager()
{
	stopCurrentScene();
	LOG_INFO("Bye ESceneManager !");
}

bool ESceneManager::playScene(std::string scene_name)
{
	std::shared_ptr<Layer> scene;
	EResourceFactory& resFactory = Ecore::getInstance()->getResourceFactory();

	if (nullptr == currentScene) {
		scene = resFactory.createScene(scene_name);
		LOG_INFO("   Play scene [%s] / %p", scene_name.c_str(), scene.get());

		currentScene = scene;
	} else {
		LOG_ERR("Scene [%s] is now playing", currentScene->getName().c_str());

		stopCurrentScene();
		scene = resFactory.createScene(scene_name);
		LOG_INFO("   Play scene [%s] / %p", scene_name.c_str(), scene.get());

		currentScene = scene;
	}

	return true;
}

void ESceneManager::stopCurrentScene()
{
	if (currentScene) {
		EResourceManager& resMgr = Ecore::getInstance()->getResourceManager();

		currentScene = nullptr;
		resMgr.updateImageResourceCache();
	}
}

void ESceneManager::pauseCurrentScene()
{
	if (currentScene)
		currentScene->setActiveState(false);
}

void ESceneManager::startCurrentScene()
{
	if (currentScene)
		currentScene->setActiveState(true);
}

void ESceneManager::initDebugScene()
{
	EResourceManager& resMgr = Ecore::getInstance()->getResourceManager();

	LOG_DBG("Create debug overlay scene");
	debug_overlay = resMgr.createScene(SCENE_DEBUG, "debug");
	if (nullptr == debug_overlay) {
		LOG_ERR("Failed to create debug scene !");
	}
}

void ESceneManager::handleEvent(SDL_Event e)
{
	/* Handler inter-scene events */
	bool ret = false;
	std::string default_scene("main");
	static int ax = 0, ay = 0;

	if (e.type == SDL_KEYDOWN) {
		//LOG_INFO("Event : [%x] / Pressed [%d]", e.type, e.key.keysym.sym);
		switch (e.key.keysym.sym) {
		case SDLK_1:
			LOG_INFO("Play Scene");
			ret = playScene(default_scene);
			if (false == ret)
				LOG_ERR("Failed to play scene [%s] !", default_scene.c_str());
			break;
		case SDLK_2:
			stopCurrentScene();
			break;
		case SDLK_3:
			startCurrentScene();
			break;
		case SDLK_4:
			pauseCurrentScene();
			break;
		case SDLK_BACKQUOTE:
			if (overlayState) {
				EResourceManager& resMgr = Ecore::getInstance()->getResourceManager();
				overlayState = false;
				overlay = nullptr;
				resMgr.updateImageResourceCache();
			}
			else {
				overlayState = true;
				EResourceFactory& resFactory = Ecore::getInstance()->getResourceFactory();
				std::string sname = "vnovel";

				overlay = resFactory.createScene(sname);
				LOG_INFO("   Overlay scene [%s] / %p", sname.c_str(), overlay.get());
			}

			break;
		}
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN)
	{
	}
	else if (e.type == SDL_FINGERUP)
	{
	}
	else if (e.type == SDL_FINGERDOWN) {
		SDL_TouchFingerEvent *te = &e.tfinger;
		LOG_INFO("Handle event! type: SDL_FINGERDOWN / %u", te->timestamp);

		if (te->x >= 0.3 && te-> x <= 0.6 && te->y >= 0.7) {
			if (overlayState) {
				EResourceManager& resMgr = Ecore::getInstance()->getResourceManager();
				overlayState = false;
				overlay = nullptr;
				resMgr.updateImageResourceCache();
			}
			else {
				overlayState = true;
				EResourceFactory& resFactory = Ecore::getInstance()->getResourceFactory();
				std::string sname = "vnovel";
				
				overlay = resFactory.createScene(sname);
				LOG_INFO("   Overlay scene [%s] / %p", sname.c_str(), overlay.get());
			}
		}
	}

	/* Propagate event into Scene instance */
	if (currentScene) {
		currentScene->handleEvent(e);
	}
	if (overlay) {
		overlay->handleEvent(e);
	}
#if 0
	static Uint32 latestEventTime = 0;
	//LOG_INFO("texture handle event!");
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		SDL_MouseButtonEvent *me = &e.button;
		LOG_INFO("Handle event! type: %d", e.button.button);
		if (me->button == SDL_BUTTON_LEFT) {
			createTexture(me->x, me->y);
			propagateEvent(e);
		}
		else if (me->button == SDL_BUTTON_RIGHT) {
			removeTexture();
		}
	} else if (e.type == SDL_FINGERDOWN) {
		SDL_TouchFingerEvent *te = &e.tfinger;
		int screen_width = 0;
		int screen_height = 0;
		SDL_Window *window = Ecore::getInstance()->getWindow();
		if (window == NULL) {
			LOG_ERR("window is NULL!");
			return;
		}

		LOG_INFO("Handle event! type: SDL_FINGERDOWN / %u", te->timestamp);

		/* Limits event handling time */
		if (te->timestamp - latestEventTime < 200) {
			return;
		}
		latestEventTime = te->timestamp;

		SDL_GetWindowSize(window, &screen_width, &screen_height);
		LOG_INFO("x / y : [%f / %f]",
			te->x * screen_width, te->y * screen_height);
		createTexture((int)te->x * screen_width, (int)te->y * screen_height);
		propagateEvent(e);
	} else if (e.type == SDL_FINGERMOTION) {
		/* Touch & swipe clears textures */
		SDL_TouchFingerEvent *te = &e.tfinger;
		int ax = (int)((te->dx > 0.0) ? te->dx : te->dx * -1.0) * 1000;
		int ay = (int)((te->dy > 0.0) ? te->dy : te->dy * -1.0) * 1000;

		LOG_INFO("Handle event! type: SDL_FINGERMOTION");
		LOG_INFO("dx / dy : [%f / %f]", te->dx, te->dy);
		LOG_INFO("ax / ay : [%d / %d]", ax, ay);

		if (ax + ay > 50)
			removeTexture();
	} else if (e.type == SDL_KEYDOWN) {
		LOG_INFO("Pressed [%d]", e.key.keysym.sym);
		switch (e.key.keysym.sym) {
		case SDLK_RIGHT:
			testState &= (DIR_ALL ^ DIR_LEFT);
			testState |= DIR_RIGHT;
			LOG_INFO("Character Move : Right");
			break;
		case SDLK_LEFT:
			testState &= (DIR_ALL ^ DIR_RIGHT);
			testState |= DIR_LEFT;
			LOG_INFO("Character Move : Left");
			break;
		case SDLK_UP:
			testState &= (DIR_ALL ^ DIR_DOWN);
			testState |= DIR_UP;
			LOG_INFO("Character Move : Up");
			break;
		case SDLK_DOWN:
			testState &= (DIR_ALL ^ DIR_UP);
			testState |= DIR_DOWN;
			LOG_INFO("Character Move : Down");
			break;
		case SDLK_d:
			testBackgroundState &= (DIR_ALL ^ DIR_RIGHT);
			LOG_INFO("Background move : Right");
			break;
		case SDLK_a:
			testBackgroundState &= (DIR_ALL ^ DIR_LEFT);
			LOG_INFO("Background move : Left");
			break;
		case SDLK_w:
			testBackgroundState &= (DIR_ALL ^ DIR_UP);
			LOG_INFO("Background move : Up");
			break;
		case SDLK_s:
			testBackgroundState &= (DIR_ALL ^ DIR_DOWN);
			LOG_INFO("Background move : Down");
			break;
		}
	} else if (e.type == SDL_KEYUP) {
		/* non-character key input */
		switch (e.key.keysym.sym) {
		case SDLK_RIGHT:
			testState &= (DIR_ALL ^ DIR_RIGHT);
			break;
		case SDLK_LEFT:
			testState &= (DIR_ALL ^ DIR_LEFT);
			break;
		case SDLK_UP:
			testState &= (DIR_ALL ^ DIR_UP);
			break;
		case SDLK_DOWN:
			testState &= (DIR_ALL ^ DIR_DOWN);
			break;
		case SDLK_d:
			testBackgroundState &= (DIR_ALL ^ DIR_RIGHT);
			break;
		case SDLK_a:
			testBackgroundState &= (DIR_ALL ^ DIR_LEFT);
			break;
		case SDLK_w:
			testBackgroundState &= (DIR_ALL ^ DIR_UP);
			break;
		case SDLK_s:
			testBackgroundState &= (DIR_ALL ^ DIR_DOWN);
			break;
		}
	} else if (e.type == SDL_TEXTINPUT) {
		/* Character key treated as a text input on Windows */
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_D]) {
			testBackgroundState = true;
			LOG_INFO("Background Move : Right");
		}
	}
#endif
}

void ESceneManager::propagateEvent(SDL_Event e)
{
#if 0
	/* TODO: This logic should be executed in animation actor */
	//std::list<EDrawable*>::iterator	iter = textureList.begin();
	std::list<EFigure*>::iterator	iter = textureList.begin();
	while (iter != textureList.end()) {
		EFigure* texture = *iter;
		//texture->animateStart(Ecore::getAppTicks());
		iter++;
	}
	//textTexture->animateStart(Ecore::getAppTicks());
#endif
}

void ESceneManager::render()
{
	if (currentScene)
		currentScene->render();

	if (overlay)
		overlay->render();

	if (debug_overlay)
		debug_overlay->render();
}

void ESceneManager::update(Uint32 currentTime, Uint32 accumulator)
{
	if (currentScene)
		currentScene->update(currentTime, accumulator);

	if (overlay)
		overlay->update(currentTime, accumulator);

	if (debug_overlay)
		debug_overlay->update(currentTime, accumulator);
}

void ESceneManager::temp_moveBackGround(double dx, double dy)
{
//	background->movePositionBy(dx, dy);
}

void ESceneManager::temp_moveCharacter(double dx, double dy)
{
//	sprite->movePositionBy(dx, dy);
}
