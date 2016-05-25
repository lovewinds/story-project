#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"

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
class ESceneInfo
{
public:
	ESceneInfo(std::string name);
	virtual ~ESceneInfo();

	void handleEvent(SDL_Event e);
	void render();
	void update(Uint32 currentTime, Uint32 accumulator);

	/* Should handle creation of resources with Resource manager */
	bool addSprite(std::shared_ptr<ESprite> sprite);

	bool addImage(std::shared_ptr<EImageTexture> imgTexture);

	/* Resource manager allocates image resource */
	//std::shared_ptr<SDL_Texture_Wrap> allocateTexture(std::string path);
	//void releaseTexture(std::string path);

	bool allocateSprites();
	bool allocateImages();

	void deallocate();

	std::string getName();

	/* Export read-only resources */
	const std::map<std::string, std::shared_ptr<ESprite>>& sprite_map;
//	const std::unordered_map<std::string, std::shared_ptr<SDL_Texture_Wrap>>& texture_map;

protected:
	std::string name;

	/* Sprite map for cache
	 *   These sprites use allocated image texture on video memory.
	 */
	//std::map<std::string, std::shared_ptr<ESpriteInfo>> _sprite_map;
	std::map<std::string, std::shared_ptr<ESprite>> _sprite_map;

	std::map<std::string, std::shared_ptr<EImageTexture>> _img_texture_map;
};
