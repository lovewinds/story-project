#pragma once

#include <string>
#include <SDL.h>

#include "scene/EScene.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"
#include "texture/ETextTexture.hpp"
#include "object/animation/EAnimation.hpp"

/* Scene
 *
 * This class indicates specific scene on screen.
 *  - It should handle creation of resources to show.
 *  - It should own textures to show.
 *  - It should perform render and update.
 */
class ETitleScene : public EScene
{
public:
	ETitleScene(std::string name);
	virtual ~ETitleScene();

	/* Primitive Scene actions */
	virtual void handleEvent(SDL_Event e);
	virtual void render();
	virtual void update(Uint32 currentTime, Uint32 accumulator);

protected:
	int menu_index = 0;

	void initMenuItem();
	void sampleMenuState(std::string id);
	bool checkMenuClicked(int x, int y);
};
