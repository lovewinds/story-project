#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "resource/EScene.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"
#include "texture/ETextTexture.hpp"
#include "texture/EGridMapTexture.hpp"
#include "texture/EAnimation.hpp"
#include "texture/EGridMoveAnimation.hpp"

/* TODO: Inherit serializable class */
/*
 * This class stores only resource information for actual allocation.
 * All of these ~~Info classes are stored in ResourceManager.
 * When scene is changed, SceneManager allocates actual Scene instance
 * with this SceneInfo instance on ResourceManager.
 */

/**
 * Scene
 *
 * This class indicates specific scene on screen.
 *  - It should handle creation of resources to show.
 *  - It should own textures to show.
 *  - It should perform render and update.
 */
class ERPGScene : public EScene
{
public:
	ERPGScene(std::string name);
	virtual ~ERPGScene();

	/* Primitive Scene actions */
	virtual void handleEvent(SDL_Event e);
	virtual void render();
	virtual void update(Uint32 currentTime, Uint32 accumulator);

	virtual bool addObject(std::shared_ptr<story::Graphic::Object> object);

	virtual void setMap(std::shared_ptr<EGridMapTexture> map);

	void testAnimation(AnimationState state);
	void handleDirectonFactor(float axis_x, float axis_y);

	bool testRotate(int x, int y);

protected:
	std::shared_ptr<EGridMapTexture>	gridMap;
};
