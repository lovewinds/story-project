#include "drawable/ETextureHandler.h"

ETextureHandler* ETextureHandler::instance = NULL;

ETextureHandler::ETextureHandler()
{
	textTexture = new ETextTexture();
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

		INFO("New texture created.");
		INFO("    ( %d, %d ) / total: %d", x, y, textureList.size());
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
	INFO("texture handle event!");
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		SDL_MouseButtonEvent *me = &e.button;
		INFO("Handle event! type: %d", e.button.button);
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
			ERROR("window is NULL!");
			return;
		}

		INFO("Handle event! type: SDL_FINGERDOWN / %u", te->timestamp);

		/* Limits event handling time */
		if (te->timestamp - latestEventTime < 500) {
			return;
		}
		latestEventTime = te->timestamp;

		SDL_GetWindowSize(window, &screen_width, &screen_height);
		INFO("x / y : [%f / %f]",
			te->x * screen_width, te->y * screen_height);
		createTexture(te->x * screen_width, te->y * screen_height);
		propagateEvent(e);
	} else if (e.type == SDL_FINGERMOTION) {
		SDL_TouchFingerEvent *te = &e.tfinger;
		int ax = ((te->dx > 0.0) ? te->dx : te->dx * -1.0) * 1000;
		int ay = ((te->dy > 0.0) ? te->dy : te->dy * -1.0) * 1000;

		//INFO("Handle event! type: SDL_FINGERMOTION");
		//INFO("dx / dy : [%f / %f]", te->dx, te->dy);
		//INFO("ax / ay : [%d / %d]", ax, ay);

		if (ax + ay > 50)
			removeTexture();
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
	while (iter != textureList.end()) {
		EDrawable* texture = *iter;
		if (texture != NULL) {
			texture->render();
			//INFO("List Texture(%p) rendered !", texture);
		} else {
			INFO("List Texture is NULL. count: %d", textureList.size());
		}
		iter++;
	}

	/* TODO: Handle this texture same with others */
	textTexture->render();
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
	textTexture->update(currentTime, accumulator);
}