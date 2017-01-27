#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <memory>

#include "resource/EScene.hpp"
#include "resource/ESceneDesc.hpp"
#include "resource/ESpriteType.hpp"
#include "resource/EImageResource.hpp"
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
 * SceneDesc Functions
 */
	/* Create scene descriptor with given parameters */
	bool addSceneDesc(std::string name, std::shared_ptr<ESceneDesc> desc);

	std::shared_ptr<ESceneDesc> getSceneDesc(std::string name);

	/* Create scene with name
	 * (for some debug purpose) */
	std::shared_ptr<EScene> createScene(ESceneType type, std::string scene_name);

/*
 * Image Resource Functions
 */
	/* ResourceManager holds Image resource to handle its repetitive usage.
	 * uses 'name' as key,
	 * uses 'path' to load real image file.
	 */
	std::shared_ptr<EImageResource>
	createImageResource(std::string name, std::string path, unsigned int width, unsigned int height);

	/* Returns shared_ptr of ImageResource instance allocated */
	std::shared_ptr<EImageResource>
	getImageResource(std::string resource_name);

	void updateImageResourceCache();

	std::shared_ptr<EImageTexture>
	createImageTexture(std::string name, std::string base_image);

/*
 * Sprite Functions
 */
	/* Create Sprite type template */
	bool createSpriteType(std::shared_ptr<ESpriteType>);

	std::shared_ptr<ESpriteType>
	getSpriteType(std::string type_name);

	std::shared_ptr<ESprite>
	createSprite(std::string type, std::string name);

/*
 * Texture Functions
 */
	std::shared_ptr<SDLTextureWrap> allocateTexture(std::string path);

	void releaseTexture(std::string path);

protected:
	/* Allocate sprite with related image resource into memory */
	std::shared_ptr<ESprite>
	_createSpriteFromTypeDesc(std::string spriteID,
			std::shared_ptr<ESpriteType> spriteType);

	EResourceLoaderInterface *loader;

	/* Store sprite info here for caching */
	std::map<std::string, std::shared_ptr<ESpriteType>> sprite_types;

	std::map<std::string, std::shared_ptr<ESceneDesc>> scene_desc_map;

	/* image map uses path as a key, it can have simillar prefix.
	 * unordered_map can be more effecient for this case */
	std::unordered_map<std::string, std::shared_ptr<EImageResource>> image_map;
};
