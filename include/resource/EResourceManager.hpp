#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <memory>

#include "resource/ESceneInfo.hpp"
#include "resource/EImageResourceInfo.hpp"
#include "resource/ESpriteInfo.hpp"

class EResourceLoaderInterface;

class EResourceManager
{
public:
	EResourceManager();
	~EResourceManager();

	/* Prohibit copy constructor */
	EResourceManager(EResourceManager& self) = delete;

	/* Load all resources information from resource archive */
	bool loadResources(std::string& res_file);

	/* Create scene with name and ready for allocation */
	std::shared_ptr<ESceneInfo> createScene(std::string scene_name);

	/* ResourceManager holds Image resource to handle its repetitive usage.
	 * uses 'name' as key,
	 * uses 'path' to load real image file.
	 */
	bool createImageResource(std::string name, std::string path);

	/* ResourceManager holds sprite resource to handle its repetitive usage */
	bool createSprite(std::string name, std::string src_image_name);

	/* Create sprite with name and ready for allocation */
	//	std::shared_ptr<ESpriteInfo>
	//	createSprite(std::string sprite_name, std::weak_ptr<EImageResourceInfo> img_resource);

	/* Allocate specific scene and related resource(i.e. sprites) into memory */
	bool allocateScene(std::string& scene_name);

	std::shared_ptr<SDL_Texture_Wrap> allocateTexture(std::string path);
	void releaseTexture(std::string path);

	/* Returns weak_ptr of ImageResource instance allocated */
	std::shared_ptr<EImageResourceInfo>	getImageResource(std::string resource_name);

protected:
	EResourceLoaderInterface *loader;

	std::map<std::string, std::shared_ptr<ESceneInfo>> scene_map;

	/* image map uses path as a key, it can have simillar prefix.
	 * unordered_map can be more effecient for this case */
	std::unordered_map<std::string, std::shared_ptr<EImageResourceInfo>> image_map;
	std::map<std::string, std::shared_ptr<ESpriteInfo>> sprite_map;

	std::unordered_map<std::string, std::shared_ptr<SDL_Texture_Wrap>> texture_map;
};
