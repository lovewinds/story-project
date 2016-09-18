#include <SDL.h>

#include <string>
#include <fstream>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/XMLResourceLoader.hpp"

#include "resource/ERPGScene.hpp"
#include "resource/EVisualNovelScene.hpp"
#include "resource/EDbgOverlayScene.hpp"

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

	if (Ecore::checkPlatform("iOS")) {
		std::ifstream f;
		f.open(path);
		if (false == f.is_open()) {
			LOG_ERR("Failed to open resource file ['%s']", path.c_str());
			return false;
		}
		f.close();
	}
	else {
		std::fstream resource(path.c_str());
		if (!resource.good()) {
			LOG_ERR("Failed to open resource file ['%s']", path.c_str());
			return false;
		}
	}

	return loader->loadResources(path);
}

std::shared_ptr<EScene> EResourceManager::createScene(ESceneType type, std::string scene_name)
{
	LOG_DBG("Trying to create scene [%s]", scene_name.c_str());
	//std::shared_ptr<EScene> scene(new EScene(scene_name));
	std::shared_ptr<EScene> scene;

	switch (type)
	{
	case SCENE_RPG:
		scene = std::shared_ptr<EScene>(new ERPGScene(scene_name));
		break;
	case SCENE_VISUAL_NOVEL:
		scene = std::shared_ptr<EScene>(new EVisualNovelScene(scene_name));
		break;
	case SCENE_DEBUG:
		scene = std::shared_ptr<EScene>(new EDbgOverlayScene(scene_name));
		break;
	default:
		/* TODO: If new scene type added, should be created here */
		return nullptr;
		break;
	}

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

std::shared_ptr<EScene> EResourceManager::getScene(std::string scene_name)
{
	std::shared_ptr<EScene> result = nullptr;
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

		/* Allocate all resources on specific scene */
		currentScene->allocate();
	} else {
		/* Scene is not found ! */
		LOG_ERR("Scene [%s] is not exist.", scene_name.c_str());
		return false;
	}

	/* TODO: Allocate sprite resources on memory */
	return true;
}

bool EResourceManager::deallocateScene(std::string scene_name)
{
	std::shared_ptr<EScene> s;
	auto scene = scene_map.find(scene_name);

	if (scene != scene_map.end()) {
		s = scene->second;
		s->deallocate();
	} else {
		/* Scene is not found ! */
		LOG_ERR("Scene [%s] is not exist.", scene_name.c_str());
		return false;
	}

	return true;
}

std::shared_ptr<SDL_Texture_Wrap> EResourceManager::allocateTexture(std::string path)
{
#if 0
	std::shared_ptr<SDL_Texture_Wrap> result;
	if (currentScene)
		result = currentScene->allocateTexture(path);
#endif
	std::shared_ptr<SDL_Texture_Wrap> result;
	auto found = _texture_map.find(path);

	if (found == _texture_map.end()) {
		/* Texture is not exist. allocate one */
		std::shared_ptr<SDL_Texture_Wrap> texture(new SDL_Texture_Wrap(path));
		if (texture) {
			//auto insert = _texture_map.emplace(path, texture);
			std::pair<std::string, std::shared_ptr<SDL_Texture_Wrap>> p(path, texture);
			auto insert = _texture_map.insert(p);
			//auto insert = texture_map.insert(path, texture);
			if (insert.second) {
				// stored
				result = texture;
			} else {
				LOG_ERR("Failed to insert texture !");
				int idx = 0;

				for(auto& t : _texture_map)
				{
					//auto t = it.second.get();
					//SDL_Texture *tx = t.second.get()->getTexture();
				}
				result = nullptr;
			}
		} else {
			LOG_ERR("Failed to allocate texture !");
		}
	} else {
		result = found->second;
	}

	return result;
}

std::shared_ptr<SDL_Texture_Wrap>
EResourceManager::allocateTextTexture(std::string text, SDL_Color textColor, SDL_Color bgColor)
{
	std::shared_ptr<SDL_Texture_Wrap> texture(new SDL_Texture_Wrap(text, textColor, bgColor));
	LOG_ERR("[ResMgr] texture [%p]", &texture);
	return texture;
}

// TODO: Need to deallocate texture here if caching is applied.
void EResourceManager::releaseTexture(std::string path)
{
	LOG_ERR("  Count of texture map items [%lu]", _texture_map.size());
	if (_texture_map.empty())
		return;

	std::shared_ptr<SDL_Texture_Wrap> result;
	auto found = _texture_map.find(path);

	if (found != _texture_map.end()) {
		LOG_DBG("  texture [%s] count [%lu]", found->first.c_str(), found->second.use_count());
		/* Reference counting logic
		 * If use_count is 1, it is not owned by all activated scenes.
		 * remove it ! */
		if (found->second.use_count() == 1)
			_texture_map.erase(found);
	}
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

std::shared_ptr<EImageResource>
EResourceManager::createImageResource(std::string name, std::string path, unsigned int width, unsigned int height)
{
	LOG_DBG("Trying to create image resource [%s]", name.c_str());
	std::shared_ptr<EImageResource> imgInfo(new EImageResource(name, path, width, height));

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

std::shared_ptr<EImageResource>
EResourceManager::getImageResource(std::string resource_name)
{
	/* TDOO: Resolve key issue - use alias? path? */
	//std::weak_ptr<EImageResource>	imgRes = image_map.find(resource_name);
	std::shared_ptr<EImageResource> found;
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