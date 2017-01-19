#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <memory>

#include "resource/EScene.hpp"
#include "resource/ESpriteType.hpp"
#include "resource/EImageResource.hpp"
#include "texture/ESprite.hpp"

class EResourceLoaderInterface;

class EImageResource
{
public:
	EImageResource(std::string name, std::string path, std::string sourceType,
			int width, int height);

protected:
	std::string name;
	std::string path;
	std::string sourceType;
	int width;
	int height;
}

class EResourceFactory
{
public:
	EResourceFactory();
	~EResourceFactory();

	/* Prohibit copy constructor */
	EResourceFactory(EResourceFactory& self) = delete;

/*
 * Scene Functions
 */
	/* Allocate specific scene and related resource(i.e. sprites) into memory */
	bool allocateScene(std::string scene_name);

	/* De-Allocate specific scene and related resource(i.e. sprites) from memory */
	bool deallocateScene(std::string scene_name);

/*
 * Image Resource Functions
 */
	/* ResourceManager holds Image resource to handle its repetitive usage.
	 * uses 'name' as key,
	 * uses 'path' to load real image file.
	 */
	bool createImageResource(std::string name, std::string path,
			unsigned int width, unsigned int height);

	/* Returns shared_ptr of ImageResource instance allocated */
	std::shared_ptr<EImageResource>
	getImageResource(std::string resource_name);

	std::shared_ptr<EImageTexture>
	createImageTexture(std::string name, std::string base_image);

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
	//	createSprite(std::string sprite_name, std::weak_ptr<EImageResource> img_resource);

/*
 * Text texture Functions
 */
	std::shared_ptr<SDLTextureWrap>
	allocateTextTexture(std::string text, SDL_Color textColor, SDL_Color bgColor);

/*
 * Grid texture map
 */
 	std::shared_ptr<SDLTextureWrap>
	allocateGridMapTexture(std::vector<std::vector<short> > map);

/*
 * Texture Functions
 */
	std::shared_ptr<SDLTextureWrap> allocateTexture(std::string path);

	void releaseTexture(std::string path);

protected:
	EResourceLoaderInterface *loader;

	/* Cache for current playing scene */
	std::shared_ptr<EScene>	currentScene;

	std::map<std::string, std::shared_ptr<EScene>> scene_map;

	/* image map uses path as a key, it can have simillar prefix.
	 * unordered_map can be more effecient for this case */
	std::unordered_map<std::string, std::shared_ptr<EImageResource>> image_map;

	/* Store sprite info here for caching */
	std::map<std::string, std::shared_ptr<ESpriteType>> sprite_types;

	/* Allocated sprite instances */
	std::map<std::string, std::shared_ptr<ESprite>> _sprite_map;

	/* Allocated Image Texture instances */
	std::map<std::string, std::shared_ptr<EImageTexture>> _image_texture_map;

	/* Texture map for cache
	 *   These textures are already allocated on video memory.
	 */
	std::unordered_map<std::string, std::shared_ptr<SDLTextureWrap>> _texture_map;
};
