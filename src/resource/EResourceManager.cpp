#include <SDL.h>

#include <string>
#include <fstream>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/XMLResourceLoader.hpp"
#include "resource/ESpriteInfo.hpp"

EResourceManager::EResourceManager()
{
	/* TODO: Get specific loader from factory class */
	loader = new XMLResourceLoader(this);
	SDL_assert(loader);
}

EResourceManager::~EResourceManager()
{
	LOG_DBG("Bye ResourceManager !");
}

bool EResourceManager::loadResources(std::string& res_file)
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

bool EResourceManager::allocateScene(std::string& scene_name)
{
	/* TODO: Search target scene with scene name */
	//std::shared_ptr<ESceneInfo>	scene = scene_map[scene_name];
	auto scene = scene_map.find(scene_name);

	if (scene != scene_map.end()) {
		/* Scene found. Allocate Image resources */
		for (auto& it : image_map) {
			std::string path(Ecore::getResourcePath(it.second->getName()));
			LOG_INFO("   Stored path: [%s]", it.second->getName().c_str());
			LOG_INFO("   Image  path: [%s]", path.c_str());

			//it.second->allocate();
		}
	} else {
		/* not found */
		return false;
	}

	/* TODO: Allocate sprite resources on memory */
	return true;
}

std::shared_ptr<SDL_Texture_Wrap> EResourceManager::allocateTexture(std::string path)
{
	std::shared_ptr<SDL_Texture_Wrap> result;
	auto found = texture_map.find(path);

	if (found == texture_map.end()) {
		/* Texture is not exist. allocate one */
		std::shared_ptr<SDL_Texture_Wrap> texture(new SDL_Texture_Wrap(path));
		if (texture) {
			//auto insert = texture_map.emplace(path, texture);
			std::pair<std::string, std::shared_ptr<SDL_Texture_Wrap>> p(path, texture);
			auto insert = texture_map.insert(p);
			//auto insert = texture_map.insert(path, texture);
			if (!insert.second) {
				LOG_ERR("Failed to insert texture !");
				int idx = 0;
				for(auto& t : texture_map)
				{
					//auto t = it.second.get();
					//SDL_Texture *tx = t.second.get()->getTexture();
				}
			}
			result = texture;
		} else {
			LOG_ERR("Failed to allocate texture !");
		}
	} else {
		result = found->second;
	}

	return result;
}

void EResourceManager::releaseTexture(std::string path)
{
	//auto cnt = texture_map.erase(path);
	//LOG_DBG("  Count after erase [%lu]", cnt);
	LOG_ERR("  Count before erase [%lu]", texture_map.size());
	if (texture_map.empty())
		return;

	std::shared_ptr<SDL_Texture_Wrap> result;
	auto found = texture_map.find(path);

	if (found != texture_map.end()) {
		//LOG_DBG("  texture [%s] count [%lu]", found->first.c_str(), found->second.use_count());
		//texture_map.erase(found);
	}
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

bool EResourceManager::createSprite(std::string name, std::string src_image_name)
{
	bool res = true;
	LOG_DBG("Trying to create sprite [%s] with image res[%s]", name.c_str(), src_image_name.c_str());
	std::shared_ptr<ESpriteInfo> sprite(new ESpriteInfo(name, src_image_name));

	//std::pair<std::map<std::string, std::shared_ptr<ESceneInfo>>::iterator, bool> result;
	auto result = sprite_map.emplace(name, sprite);

	if (!result.second) {
		LOG_ERR("Failed to insert sprite into set !");
		res = false;
	}
#if 1
	LOG_INFO("Current sprites:");
	for(auto& it : sprite_map)
	{
		//auto t = it.second.get();
		LOG_INFO("   %s", it.second->getName().c_str());
	}
#endif
	return res;
}

bool EResourceManager::createImageResource(std::string name, std::string path)
{
	bool result = false;
	LOG_DBG("Trying to create image resource [%s]", name.c_str());
	std::shared_ptr<EImageResourceInfo> imgInfo(new EImageResourceInfo(name, path));

	//std::pair<std::map<std::string, std::shared_ptr<ESceneInfo>>::iterator, bool> result;
	auto map_result = image_map.emplace(name, imgInfo);

	if (map_result.second) {
		result = true;
	} else {
		LOG_ERR("Failed to insert image resource map !");
	}

	LOG_INFO("Current Image resources:");
	for(auto& it : image_map)
	{
		//auto t = it.second.get();
		LOG_INFO("   %s", it.second->getName().c_str());
	}

	return result;
}

std::shared_ptr<EImageResourceInfo>	EResourceManager::getImageResource(std::string resource_name)
{
	/* TDOO: Resolve key issue - use alias? path? */
	//std::weak_ptr<EImageResourceInfo>	imgRes = image_map.find(resource_name);
	std::shared_ptr<EImageResourceInfo> found;
	auto imgRes = image_map.find(resource_name);

	if (imgRes != image_map.end()) {
		/* ImageResource found */

		// Return weak_ptr of imageResource
		found = imgRes->second;
	}

	/* not found */
	return found;
}
