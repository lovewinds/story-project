#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <memory>

#include "resource/ESceneInfo.hpp"
#include "resource/ESpriteType.hpp"
#include "resource/EImageResourceInfo.hpp"
#include "texture/ESprite.hpp"

class EResourceLoaderInterface;

class EResourceManager
{
public:
	EResourceManager();
	~EResourceManager();

	/* Prohibit copy constructor */
	EResourceManager(EResourceManager& self) = delete;

	/* Load all resources information from resource archive */
	bool loadResources(std::string res_file);

/*
 * Scene Functions
 */
	/* Create scene with name and ready for allocation */
	std::shared_ptr<ESceneInfo> createScene(std::string scene_name);
	std::shared_ptr<ESceneInfo> getScene(std::string scene_name);

	/* Allocate specific scene and related resource(i.e. sprites) into memory */
	bool allocateScene(std::string scene_name);

/*
 * Image Resource Functions
 */
	/* ResourceManager holds Image resource to handle its repetitive usage.
	 * uses 'name' as key,
	 * uses 'path' to load real image file.
	 */
	std::shared_ptr<EImageResourceInfo>
	createImageResource(std::string name, std::string path);

	/* Returns shared_ptr of ImageResource instance allocated */
	std::shared_ptr<EImageResourceInfo>
	getImageResource(std::string resource_name);

/*
 * Sprite Functions
 */
	/* Create Sprite type template */
	bool createSpriteType(std::shared_ptr<ESpriteType>);

	std::shared_ptr<ESpriteType>
	getSpriteType(std::string type_name);

	/* ResourceManager holds sprite resource to handle its repetitive usage */
	std::shared_ptr<ESprite>
	createSprite(std::string type, std::string name);

	/* Create sprite with name and ready for allocation */
	//	std::shared_ptr<ESpriteInfo>
	//	createSprite(std::string sprite_name, std::weak_ptr<EImageResourceInfo> img_resource);

/*
 * Texture Functions
 */
	std::shared_ptr<SDL_Texture_Wrap> allocateTexture(std::string path);

	void releaseTexture(std::string path);

protected:
	EResourceLoaderInterface *loader;

	/* Cache for current playing scene */
	std::shared_ptr<ESceneInfo>	currentScene;

	std::map<std::string, std::shared_ptr<ESceneInfo>> scene_map;

	/* image map uses path as a key, it can have simillar prefix.
	 * unordered_map can be more effecient for this case */
	std::unordered_map<std::string, std::shared_ptr<EImageResourceInfo>> image_map;

	/* Store sprite info here for caching */
	std::map<std::string, std::shared_ptr<ESpriteType>> sprite_types;

	/* Allocated sprite instances */
	std::map<std::string, std::shared_ptr<ESprite>> _sprite_map;

	/* TODO: Store texture here for caching */
	//std::unordered_map<std::string, std::shared_ptr<SDL_Texture_Wrap>> texture_map;
};
