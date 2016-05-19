#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "texture/ESprite.hpp"

#include "resource/ESceneInfo.hpp"

ESceneInfo::ESceneInfo(std::string name)
: texture_map(_texture_map),
  sprite_map(_sprite_map)
{
	LOG_INFO("ESceneInfo[%s] created", name.c_str());
	this->name = name;
}

ESceneInfo::~ESceneInfo()
{
	_sprite_map.clear();
	_texture_map.clear();

	LOG_INFO("ESceneInfo[%s] removed", name.c_str());
}

std::string ESceneInfo::getName()
{
	return name;
}

std::shared_ptr<SDL_Texture_Wrap>
ESceneInfo::allocateTexture(std::string path)
{
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

void ESceneInfo::releaseTexture(std::string path)
{
	//auto cnt = _texture_map.erase(path);
	//LOG_DBG("  Count after erase [%lu]", cnt);
	LOG_ERR("  Count before erase [%lu]", _texture_map.size());
	if (_texture_map.empty())
		return;

	std::shared_ptr<SDL_Texture_Wrap> result;
	auto found = _texture_map.find(path);

	if (found != _texture_map.end()) {
		//LOG_DBG("  texture [%s] count [%lu]", found->first.c_str(), found->second.use_count());
		//_texture_map.erase(found);
	}
}

bool ESceneInfo::addSprite(std::shared_ptr<ESprite> sprite)
{
#if 0
	LOG_DBG("Trying to create sprite type [%s] / name [%s]", type.c_str(), name.c_str());

	/* TODO: Move allocation logic into Resource Manager */
	//std::shared_ptr<ESprite> sprite(new ESprite(name, base_image));
	EResourceManager& resMgr = Ecore::getInstance()->getResourceManager();
	std::shared_ptr<ESprite> sprite = resMgr.createSprite(type, name);

	//std::pair<std::map<std::string, std::shared_ptr<ESceneInfo>>::iterator, bool> result;
	auto result = _sprite_map.emplace(name, sprite);

	if (!result.second) {
		LOG_ERR("Failed to insert sprite into set !");
		return nullptr;
	}
#if 1
	LOG_INFO("Current sprites:");
	for(auto& it : _sprite_map)
	{
		//auto t = it.second.get();
		LOG_INFO("   %s", it.first.c_str());
	}
#endif
	return sprite;
#endif
	auto result = _sprite_map.emplace(sprite->getName(), sprite);
	if (!result.second) {
		LOG_ERR("Failed to insert sprite set !");
		return false;
	}
	return true;
}

bool ESceneInfo::addImage(std::shared_ptr<EImageTexture> imgTexture)
{
	auto result = _img_texture_map.emplace(imgTexture->getName(), imgTexture);
	if (!result.second) {
		LOG_ERR("Failed to insert imgTexture set !");
		return false;
	}
	return true;
}

bool ESceneInfo::allocateSprites()
{
	if (_sprite_map.empty())
		return false;

	for(auto &it : _sprite_map) {
		LOG_DBG("  Allocate sprite [%s] count [%lu]", it.first.c_str(), it.second.use_count());
		it.second->allocate();
	}
	return true;
}

bool ESceneInfo::allocateImages()
{
	if (_img_texture_map.empty())
		return false;

	for(auto &it : _img_texture_map) {
		LOG_DBG("  Allocate image texture [%s] count [%lu]", it.first.c_str(), it.second.use_count());
		it.second->allocate();
	}
	return true;
}

void ESceneInfo::handleEvent(SDL_Event e)
{

}

void ESceneInfo::render()
{
	for (auto &it : _img_texture_map)
	{
		it.second->render();
	}

	for (auto& it : _sprite_map)
	{
		it.second->render();
	}
#if 0
	std::list<EDrawable*>::iterator	iter = textureList.begin();
	Uint32 current = SDL_GetTicks();

	/* TODO: Handle this texture same with others */
	background->render();

	while (iter != textureList.end()) {
		EDrawable* texture = *iter;
		if (texture != NULL) {
			texture->render();
			//LOG_INFO("List Texture(%p) rendered !", texture);
		} else {
			LOG_INFO("List Texture is NULL. count: %lu", textureList.size());
		}
		iter++;
	}
	textTexture->render();
	sprite->render();
#endif
}

void ESceneInfo::update(Uint32 currentTime, Uint32 accumulator)
{
	for(auto& it : _sprite_map)
	{
		it.second->update(currentTime, accumulator);
	}
#if 0
	//std::list<ETexture*>::iterator	iter = textureList.begin();
	std::list<EDrawable*>::iterator	iter = textureList.begin();
	while (iter != textureList.end()) {
		EDrawable* texture = *iter;
		if (texture != NULL) {
			//texture->calculate(currentTime, accumulator);
			texture->update(currentTime, accumulator);
		}
		iter++;
	}

	/* TODO: Handle this texture same with others */
	background->update(currentTime, accumulator);

	textTexture->update(currentTime, accumulator);
	sprite->update(currentTime, accumulator);

	/* TEST */
	if (testState & DIR_UP)
		temp_moveCharacter(0.0, -1.0);
	if (testState & DIR_DOWN)
		temp_moveCharacter(0.0, 1.0);
	if (testState & DIR_LEFT)
		temp_moveCharacter(-1.0, 0.0);
	if (testState & DIR_RIGHT)
		temp_moveCharacter(1.0, 0.0);

	if (testBackgroundState & DIR_UP)
		temp_moveBackGround(0.0, -1.0);
	if (testBackgroundState & DIR_DOWN)
		temp_moveBackGround(0.0, 1.0);
	if (testBackgroundState & DIR_LEFT)
		temp_moveBackGround(-1.0, 0.0);
	if (testBackgroundState & DIR_RIGHT)
		temp_moveBackGround(1.0, 0.0);
#endif
}
