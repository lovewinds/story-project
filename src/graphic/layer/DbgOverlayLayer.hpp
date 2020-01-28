#pragma once

#include <string>

#include "util/SDLWrap.hpp"
#include "scene/EScene.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"
#include "texture/ETextTexture.hpp"
#include "object/animation/EAnimation.hpp"

/* TODO: Inherit serializable class */
/*
* This class stores only resource information for actual allocation.
* All of these ~~Info classes are stored in ResourceManager.
* When scene is changed, SceneManager allocates actual Scene instance
* with this SceneInfo instance on ResourceManager.
*/

/* Scene
*
* This class indicates specific scene on screen.
*  - It should handle creation of resources to show.
*  - It should own textures to show.
*  - It should perform render and update.
*/
class EDbgOverlayScene : public Layer
{
public:
	EDbgOverlayScene(std::string name);
	virtual ~EDbgOverlayScene();

	/* Primitive Scene actions */
	virtual void handleEvent(SDL_Event e);
	virtual void render();
	virtual void update(Uint32 currentTime, Uint32 accumulator);
};
