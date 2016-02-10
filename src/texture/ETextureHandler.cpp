#include "texture/ETextureHandler.h"

ETextureHandler* ETextureHandler::instance = NULL;

ETextureHandler::ETextureHandler()
{
	textTexture = new ETextTexture();
	sprite = new ESprite();
	background = new EImageTexture();

	if (false == sprite->loadFromFile("../res/adol-sprite.png"))
	{
		/* TEMPORARY: For Android */
		sprite->loadFromFile("adol-sprite.png");
	}

	if (false == background->loadFromFile("../res/room_x4.png"))
	{
		/* TEMPORARY: For Android */
		background->loadFromFile("room_x4.png");
	}

	/* TEMP: Input state handle */
	testState = DIR_STOP;
	testBackgroundState = DIR_STOP;
}

ETextureHandler::~ETextureHandler()
{
	removeTexture();
}

ETextureHandler* ETextureHandler::getInstance() {
	if (instance == NULL) {
		instance = new ETextureHandler();
	}

	return instance;
}

void ETextureHandler::createTexture(int x, int y)
{
	static int maxCount = 0;
	if (maxCount < 10) {
		//ETexture*	texture = new ETexture(x, y);
		EDrawable*	texture = new EDrawable(x, y);
		texture->animateStart(SDL_GetTicks());
		textureList.push_back(texture);

		LOG_INFO("New texture created.");
		LOG_INFO("    ( %d, %d ) / total: %d", x, y, textureList.size());
	}
}

void ETextureHandler::removeTexture()
{
	//list<ETexture*>::iterator	iter;
	list<EDrawable*>::iterator	iter;
	while (textureList.empty() == false) {
		iter = textureList.begin();
		delete *iter;
		textureList.erase(iter);
	}
}

void ETextureHandler::handleEvent(SDL_Event e)
{
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

		//INFO("Handle event! type: SDL_FINGERMOTION");
		//INFO("dx / dy : [%f / %f]", te->dx, te->dy);
		//INFO("ax / ay : [%d / %d]", ax, ay);

		if (ax + ay > 50)
			removeTexture();
	} else if (e.type == SDL_KEYDOWN) {
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
		}

		/* character key input */
		switch (e.key.keysym.sym) {
		case SDL_SCANCODE_D:
			testBackgroundState &= (DIR_ALL ^ DIR_RIGHT);
			break;
		case SDL_SCANCODE_A:
			testBackgroundState &= (DIR_ALL ^ DIR_LEFT);
			break;
		case SDL_SCANCODE_W:
			testBackgroundState &= (DIR_ALL ^ DIR_UP);
			break;
		case SDL_SCANCODE_S:
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
}

void ETextureHandler::propagateEvent(SDL_Event e)
{
	//list<ETexture*>::iterator	iter = textureList.begin();
	list<EDrawable*>::iterator	iter = textureList.begin();
	while (iter != textureList.end()) {
		EDrawable* texture = *iter;
		//texture->animateStart(SDL_GetTicks());
		iter++;
	}
	//textTexture->animateStart(SDL_GetTicks());
}

void ETextureHandler::render()
{
	//list<ETexture*>::iterator	iter = textureList.begin();
	list<EDrawable*>::iterator	iter = textureList.begin();
	Uint32 current = SDL_GetTicks();

	/* TODO: Handle this texture same with others */
	background->render();

	while (iter != textureList.end()) {
		EDrawable* texture = *iter;
		if (texture != NULL) {
			texture->render();
			//LOG_INFO("List Texture(%p) rendered !", texture);
		} else {
			LOG_INFO("List Texture is NULL. count: %d", textureList.size());
		}
		iter++;
	}
	textTexture->render();
	sprite->render();
}

void ETextureHandler::update(Uint32 currentTime, Uint32 accumulator)
{
	//list<ETexture*>::iterator	iter = textureList.begin();
	list<EDrawable*>::iterator	iter = textureList.begin();
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

	if (testBackgroundState)
		temp_moveBackGround(1.0, 0.0);
}

void ETextureHandler::temp_moveBackGround(double dx, double dy)
{
	background->movePositionBy(dx, dy);
}

void ETextureHandler::temp_moveCharacter(double dx, double dy)
{
	sprite->movePositionBy(dx, dy);
}
