#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"
#include "texture/ETextTexture.hpp"
#include "texture/EAnimation.hpp"
#include "texture/EFigure.hpp"

/* TODO: Inherit serializable class */
/*
 * This class stores only resource information for actual allocation.
 * All of these ~~Info classes are stored in ResourceManager.
 * When scene is changed, SceneManager allocates actual Scene instance
 * with this SceneInfo instance on ResourceManager.
 */

enum ESceneType
{
	SCENE_RPG = 0,
	SCENE_VISUAL_NOVEL,
	SCENE_DEBUG,
};

/* Scene
 *
 * This class indicates specific scene on screen.
 *  - It should handle creation of resources to show.
 *  - It should own textures to show.
 *  - It should perform render and update.
 */
class EScene
{
public:
	EScene(std::string name);
	virtual ~EScene();

	/* Primitive Scene actions */
	virtual void handleEvent(SDL_Event e);
	virtual void render();
	virtual void update(Uint32 currentTime, Uint32 accumulator);

	/* Should handle creation of resources with Resource manager */
	bool addSprite(std::shared_ptr<ESprite> sprite);

	bool addImage(std::shared_ptr<EImageTexture> imgTexture);

	/* Resource manager allocates image resource */
	//std::shared_ptr<SDL_Texture_Wrap> allocateTexture(std::string path);
	//void releaseTexture(std::string path);

	bool allocate();
	void deallocate();

	void setActiveState(bool active);

	std::string getName();

	/* Export read-only resources */
	const std::map<std::string, std::shared_ptr<ESprite>>& sprite_map;
//	const std::unordered_map<std::string, std::shared_ptr<SDL_Texture_Wrap>>& texture_map;

protected:
	EScene();
	std::string name;

	/* TODO: Scene is allocated on memory */
	bool isAllocated;

	/* TODO: Scene is activated, so it perform update and render */
	bool isActivated;

	bool allocateSprites();
	bool allocateImages();
	bool allocateTexts();
	bool allocateRawTexture();

	/* Sprite map for cache
	 *   These sprites use allocated image texture on video memory.
	 */
	//std::map<std::string, std::shared_ptr<ESpriteInfo>> _sprite_map;
	std::map<std::string, std::shared_ptr<ESprite>> _sprite_map;

	std::map<std::string, std::shared_ptr<EImageTexture>> _img_texture_map;
	//std::map<std::string, std::shared_ptr<EImageResource>> _img_resource_map;

	std::map<std::string, std::shared_ptr<ETextTexture>> _text_texture_map;

	std::map<std::string, std::shared_ptr<EFigure>> _drawable_map;

	/* Not classified textures */
	std::map<std::string, std::shared_ptr<EDrawable>> _raw_texture_map;
};
