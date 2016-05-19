#include <SDL.h>

#include <string>
#include <fstream>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/XMLResourceLoader.hpp"

EResourceManager::EResourceManager()
{
	/* TODO: Get specific loader from factory class */
	loader = new XMLResourceLoader(this);
	SDL_assert(loader);
}

EResourceManager::~EResourceManager()
{
	if (loader) {
		delete loader;
		loader = NULL;
	}

	scene_map.clear();
	image_map.clear();

	LOG_DBG("Bye ResourceManager !");
}

bool EResourceManager::loadResources(std::string res_file)
{
	std::string path = Ecore::getResourcePath(res_file);
	if (path.empty()) {
		LOG_ERR("Empty resource file path !");
		return false;
	}
	LOG_DBG("Load resource file [%s]", path.c_str());

	std::fstream resource(path.c_str());
	if (!resource.good()) {
		LOG_ERR("Failed to open resource file ['%s']", path.c_str());
		return false;
	}

	return loader->loadResources(path);
}

std::shared_ptr<ESceneInfo> EResourceManager::createScene(std::string scene_name)
{
	LOG_DBG("Trying to create scene [%s]", scene_name.c_str());
	std::shared_ptr<ESceneInfo> scene(new ESceneInfo(scene_name));

	//std::pair<std::map<std::string, std::shared_ptr<ESceneInfo>>::iterator, bool> result;
	auto result = scene_map.emplace(scene_name, scene);

	if (!result.second) {
		LOG_ERR("Failed to insert scene set !");
	}
#if 0
	LOG_INFO("Current scenes:");
	for(auto& it : scene_map)
	{
		//auto t = it.second.get();
		LOG_INFO("   %s", it.second->getName().c_str());
	}
#endif
	return scene;
}

std::shared_ptr<ESceneInfo> EResourceManager::getScene(std::string scene_name)
{
	std::shared_ptr<ESceneInfo> result = nullptr;
	auto scene = scene_map.find(scene_name);

	if (scene != scene_map.end()) {
		result = scene->second;
	} else {
		result = nullptr;
	}
	return result;
}

bool EResourceManager::allocateScene(std::string scene_name)
{
	/* Search target scene with scene name */
	auto scene = scene_map.find(scene_name);
	if (scene != scene_map.end()) {
		/* Scene found. Allocate Image resources */
		currentScene = scene->second;

		for (auto& it : image_map) {
			std::string name(it.first);
			std::string path(it.second->getPath());
			LOG_INFO("   Stored name: [%s]", name.c_str());
			LOG_INFO("   Image  path: [%s]", path.c_str());

			it.second->allocate();
		}

		/* TODO: Allocate all sprites and image textures on specific scene */
		currentScene->allocateSprites();
		currentScene->allocateImages();
	} else {
		/* Scene is not found ! */
		LOG_ERR("Scene [%s] is not exist.", scene_name.c_str());
		return false;
	}

	/* TODO: Allocate sprite resources on memory */
	return true;
}

std::shared_ptr<SDL_Texture_Wrap> EResourceManager::allocateTexture(std::string path)
{
	std::shared_ptr<SDL_Texture_Wrap> result;
	if (currentScene)
		result = currentScene->allocateTexture(path);

	return result;
}

// TODO: Need to deallocate texture here if caching is applied.
void EResourceManager::releaseTexture(std::string path)
{
	// Get current scene
	if (currentScene)
		currentScene->releaseTexture(path);
}

bool EResourceManager::createSpriteType(std::shared_ptr<ESpriteType> spriteType)
{
	if (!spriteType)
		return false;

	auto inserted = sprite_types.emplace(spriteType->getName(), spriteType);
	if (!inserted.second) {
		LOG_ERR("Failed to insert sprite type map!");
		return false;
	}

	return true;
}

std::shared_ptr<ESpriteType>
EResourceManager::getSpriteType(std::string type_name)
{
	std::shared_ptr<ESpriteType> found = nullptr;
	auto spriteType = sprite_types.find(type_name);

	if (spriteType != sprite_types.end()) {
		/* Found */
		found = spriteType->second;
	}

	return found;
}

std::shared_ptr<ESprite>
EResourceManager::createSprite(std::string type, std::string name)
{
	LOG_DBG("Trying to create sprite type [%s] / name [%s]", type.c_str(), name.c_str());

	//std::shared_ptr<ESprite> sprite(new ESprite(name, base_image));
	/* Find sprite template type */

	//std::pair<std::map<std::string, std::shared_ptr<ESpriteType>>::iterator, bool> result;
	auto t = sprite_types.find(type);
	if (t == sprite_types.end()) {
		LOG_ERR("Sprite type [%s] is not defined.", type.c_str());
		return nullptr;
	}

	std::shared_ptr<ESpriteType> spriteType = t->second;
	if (!spriteType) {
		LOG_ERR("Sprite type [%s] is invalid.", t->first.c_str());
		return nullptr;
	}
	std::shared_ptr<ESprite> sprite = spriteType->createSprite(name);
	if (!sprite) {
		LOG_ERR("Failed to allocate sprite! [Type:%s]", spriteType->getName().c_str());
		return nullptr;
	}

	//std::pair<std::map<std::string, std::shared_ptr<ESceneInfo>>::iterator, bool> result;
	auto result = _sprite_map.emplace(name, sprite);

	if (!result.second) {
		LOG_ERR("Failed to insert sprite into set !");
		return nullptr;
	}

#if 1
	LOG_DBG("Current sprites:");
	for(auto& it : _sprite_map)
	{
		//auto t = it.second.get();
		LOG_DBG("   %s", it.first.c_str());
	}
#endif
	return sprite;
}

std::shared_ptr<EImageResourceInfo>
EResourceManager::createImageResource(std::string name, std::string path, unsigned int width, unsigned int height)
{
	LOG_DBG("Trying to create image resource [%s]", name.c_str());
	std::shared_ptr<EImageResourceInfo> imgInfo(new EImageResourceInfo(name, path, width, height));

	if (!imgInfo) {
		LOG_ERR("Failed to create Image Resource info!!");
		return nullptr;
	}

	//std::pair<std::map<std::string, std::shared_ptr<ESceneInfo>>::iterator, bool> result;
	auto map_result = image_map.emplace(name, imgInfo);

	if (map_result.second) {
		LOG_INFO("Created Image resource info !");
	} else {
		LOG_ERR("Failed to insert image resource map !");
	}
#if 1
	LOG_INFO("Current Image resources:");
	for(auto& it : image_map)
	{
		//auto t = it.second.get();
		LOG_INFO("   %s", it.second->getName().c_str());
	}
#endif
	return imgInfo;
}

std::shared_ptr<EImageResourceInfo>
EResourceManager::getImageResource(std::string resource_name)
{
	/* TDOO: Resolve key issue - use alias? path? */
	//std::weak_ptr<EImageResourceInfo>	imgRes = image_map.find(resource_name);
	std::shared_ptr<EImageResourceInfo> found;
	auto imgRes = image_map.find(resource_name);

	if (imgRes != image_map.end()) {
		/* ImageResource found */

		// Return weak_ptr of imageResource
		found = imgRes->second;
	} else {
		found = nullptr;
	}

	/* not found */
	return found;
}

std::shared_ptr<EImageTexture>
EResourceManager::createImageTexture(std::string name, std::string base_image)
{
	std::shared_ptr<EImageTexture> imgTexture(new EImageTexture(name, base_image));
	if (!imgTexture) {
		LOG_ERR("Failed to create Image texture !!");
		return nullptr;
	}

	//std::pair<std::map<std::string, std::shared_ptr<EImageTexture>>::iterator, bool> result;
	auto map_result = _image_texture_map.emplace(name, imgTexture);

	if (map_result.second) {
		LOG_INFO("Created Image texture !!");
	} else {
		LOG_ERR("Failed to insert image texture map !");
	}

#if 1
	LOG_INFO("Current Image textures:");
	for(auto& it : _image_texture_map)
	{
		//auto t = it.second.get();
		LOG_INFO("   %s", it.second->getName().c_str());
	}
#endif

	return imgTexture;
}
