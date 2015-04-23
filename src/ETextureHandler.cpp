#include "ETextureHandler.h"

ETextureHandler* ETextureHandler::instance = NULL;

ETextureHandler::ETextureHandler()
{
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
	if (maxCount == 0) {
		ETexture*	texture = new ETexture(x, y);
		textureList.push_back(texture);
	}
}

void ETextureHandler::removeTexture()
{
	textureList.clear();
}

void ETextureHandler::handleEvent(SDL_Event e)
{
	SDL_MouseButtonEvent *me = &e.button;

	if (me->button == SDL_BUTTON_LEFT) {
		createTexture(me->x, me->y);
		propagateEvent(e);
	}
	else if (me->button == SDL_BUTTON_RIGHT) {
		removeTexture();
	}
}

void ETextureHandler::propagateEvent(SDL_Event e)
{
	list<ETexture*>::iterator	iter = textureList.begin();
	while (iter != textureList.end()) {
		ETexture* texture = *iter;
		texture->animateStart(SDL_GetTicks());
		iter++;
	}
}