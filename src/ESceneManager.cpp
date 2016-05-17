#include "ESceneManager.hpp"

#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"

//ESceneManager* ESceneManager::instance = NULL;

ESceneManager::ESceneManager()
{
	currentScene = nullptr;
#if 0
	textTexture = new ETextTexture();

	//sprite = new ESprite();
	//background = new EImageTexture();

	if (false == sprite->loadFromFile("../res/moderngirl02x2.png"))
	{
		/* TEMPORARY: For Android */
		sprite->loadFromFile("moderngirl02.png");
	}

	if (false == background->loadFromFile("../res/japanvillage.jpg"))
	{
		/* TEMPORARY: For Android */
		background->loadFromFile("japanvillage.jpg");
	}
#endif
	/* TEMP: Input state handle */
	testState = DIR_STOP;
	testBackgroundState = DIR_STOP;
}

ESceneManager::~ESceneManager()
{
	removeTexture();
	LOG_INFO("Bye ESceneManager !");
}
#if 0
bool ESceneManager::createBackgroundImage(std::string img_path)
{
	bool created = false;
	background = new EImageTexture(img_path);

	created = background->loadFromFile(img_path.c_str());

	return created;
}

bool ESceneManager::createSprite(std::string sprite_path)
{
	bool created = false;
	sprite = new ESprite();

	//created = sprite->loadFromFile(sprite_path.c_str());

	return created;
}

ESceneManager* ESceneManager::getInstance() {
	if (instance == NULL) {
		instance = new ESceneManager();
	}

	return instance;
}
#endif

void ESceneManager::createTexture(int x, int y)
{
#if 0
	static int maxCount = 0;
	if (maxCount < 10) {
		//ETexture*	texture = new ETexture(x, y);
		EDrawable*	texture = new EDrawable(x, y);
		texture->animateStart(SDL_GetTicks());
		textureList.push_back(texture);

		LOG_INFO("New texture created.");
		LOG_INFO("    ( %d, %d ) / total: %lu", x, y, textureList.size());
	}
#endif
}

void ESceneManager::removeTexture()
{
#if 0
	//std::list<ETexture*>::iterator	iter;
	std::list<EDrawable*>::iterator	iter;
	while (textureList.empty() == false) {
		iter = textureList.begin();
		delete *iter;
		textureList.erase(iter);
	}
#endif
}

bool ESceneManager::playScene(std::string scene_name)
{
	std::shared_ptr<ESceneInfo> scene;
	EResourceManager& resMgr = Ecore::getInstance()->getResourceManager();
	bool success = resMgr.allocateScene(scene_name);

	if (success) {
		scene = resMgr.getScene(scene_name);
		LOG_INFO("   Play scene [%s] / %p", scene_name.c_str(), scene.get());

		currentScene = scene;
	}

	return true;
}

void ESceneManager::handleEvent(SDL_Event e)
{
//	if (currentScene)
//		currentScene->handleEvent(e);
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
		LOG_INFO("Pressed [%d]", e.key.keysym.sym)
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
	//std::list<ETexture*>::iterator	iter = textureList.begin();
	std::list<EDrawable*>::iterator	iter = textureList.begin();
	while (iter != textureList.end()) {
		EDrawable* texture = *iter;
		//texture->animateStart(SDL_GetTicks());
		iter++;
	}
	//textTexture->animateStart(SDL_GetTicks());
#endif
}

void ESceneManager::render()
{
	if (currentScene)
		currentScene->render();
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

void ESceneManager::update(Uint32 currentTime, Uint32 accumulator)
{
	if (currentScene)
		currentScene->update(currentTime, accumulator);
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

void ESceneManager::temp_moveBackGround(double dx, double dy)
{
//	background->movePositionBy(dx, dy);
}

void ESceneManager::temp_moveCharacter(double dx, double dy)
{
//	sprite->movePositionBy(dx, dy);
}
