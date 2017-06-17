#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "scene/EScene.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"
#include "texture/ETextTexture.hpp"
#include "texture/EGridMapTexture.hpp"
#include "object/animation/EAnimation.hpp"
#include "object/animation/EGridMoveAnimation.hpp"

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
	virtual void setGridDescriptor(std::shared_ptr<EGridDesc> desc);

	void testAnimation(AnimationState state);
	bool testRotate(int x, int y);

	void handleDirectonFactor(float axis_x, float axis_y);
	bool checkGridMoveable(int x, int y);

	/* Callback when object is moved */
	void objectPositionCallback(double x, double y);

protected:
	std::shared_ptr<EGridMapTexture>	gridMap;
	std::shared_ptr<EGridDesc> gridDesc;

	/* Grid-based object list for rendering */
	//std::vector<std::list<std::shared_ptr<story::Graphic::Object>>> _vertical_obj_list;
	//std::map<std::string, std::shared_ptr<story::Graphic::Object>> _object_map;
};
