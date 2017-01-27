#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "texture/ESprite.hpp"

#include "resource/EScene.hpp"

EScene::EScene()
	: sprite_map(_sprite_map)
{
	LOG_INFO("Superclass");

	isActivated = true;
}

EScene::EScene(std::string name)
: sprite_map(_sprite_map)
{
	LOG_INFO("EScene[%s] created", name.c_str());
	this->name = name;

	isActivated = true;
}

EScene::~EScene()
{
	_sprite_map.clear();
	_text_texture_map.clear();

	isActivated = false;

	LOG_INFO("EScene[%s] removed", name.c_str());
}

std::string EScene::getName()
{
	return name;
}

/**
 * Store sprites already allocated.
 */
bool EScene::addSprite(std::shared_ptr<ESprite> sprite)
{
	auto result = _sprite_map.emplace(sprite->getName(), sprite);
	if (!result.second) {
		LOG_ERR("Failed to insert sprite map!");
		return false;
	}
	return true;
}

bool EScene::addImage(std::shared_ptr<EImageTexture> imgTexture)
{
	auto result = _img_texture_map.emplace(imgTexture->getName(), imgTexture);
	if (!result.second) {
		LOG_ERR("Failed to insert image map!");
		return false;
	}
	return true;
}

void EScene::setActiveState(bool active)
{
	isActivated = active;
}

void EScene::handleEvent(SDL_Event e)
{
	
}

void EScene::render()
{
	for (auto &it : _img_texture_map)
	{
		it.second->render();
	}

	for (auto& it : _sprite_map)
	{
		it.second->render();
	}

	for (auto& it : _text_texture_map)
	{
		it.second->render();
	}

	for (auto& it : _raw_texture_map)
	{
		it.second->render();
	}
}

void EScene::update(Uint32 currentTime, Uint32 accumulator)
{
	if (false == isActivated)
		return;

	for(auto& it : _sprite_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for(auto& it : _text_texture_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for(auto& it : _raw_texture_map)
	{
		it.second->update(currentTime, accumulator);
	}
}
