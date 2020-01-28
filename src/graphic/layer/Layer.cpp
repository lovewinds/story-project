#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "texture/ESprite.hpp"

#include "graphic/layer/Layer.hpp"

namespace story {
namespace Graphic {

Layer::Layer()
	: sprite_map(_sprite_map)
{
	LOG_INFO("Superclass");

	isActivated = true;
}

Layer::Layer(std::string name)
: sprite_map(_sprite_map)
{
	LOG_INFO("Layer[%s] created", name.c_str());
	this->name = name;

	isActivated = true;
}

Layer::~Layer()
{
	_sprite_map.clear();
	_text_texture_map.clear();
	_object_map.clear();

	isActivated = false;

	LOG_INFO("Layer[%s] removed", name.c_str());
}

std::string Layer::getName()
{
	return name;
}

bool Layer::addObject(std::shared_ptr<story::Graphic::Object> object)
{
	auto result = _object_map.emplace(object->getName(), object);
	if (!result.second) {
		LOG_ERR("Failed to insert object map!");
		return false;
	}
	return true;
}

#if 0
/**
 * Store sprites already allocated.
 */
bool Layer::addSprite(std::shared_ptr<ESprite> sprite)
{
	auto result = _sprite_map.emplace(sprite->getName(), sprite);
	if (!result.second) {
		LOG_ERR("Failed to insert sprite map!");
		return false;
	}
	return true;
}

bool Layer::addImage(std::shared_ptr<EImageTexture> imgTexture)
{
	auto result = _img_texture_map.emplace(imgTexture->getName(), imgTexture);
	if (!result.second) {
		LOG_ERR("Failed to insert image map!");
		return false;
	}
	return true;
}
#endif

void Layer::setActiveState(bool active)
{
	isActivated = active;
}

void Layer::handleEvent(SDL_Event e)
{
	
}

void Layer::render()
{
	for (auto &it : _img_texture_map)
	{
		it.second->render(0, 0, 0);
	}

	for (auto& it : _sprite_map)
	{
		it.second->render(0, 0, 0);
	}

	for (auto& it : _text_texture_map)
	{
		it.second->render(0, 0, 0);
	}

	for (auto& it : _object_map)
	{
		it.second->render();
	}

	for (auto& it : _raw_texture_map)
	{
		it.second->render(0, 0, 0);
	}
}

void Layer::update(Uint32 currentTime, Uint32 accumulator)
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

	for (auto& it : _object_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for(auto& it : _raw_texture_map)
	{
		it.second->update(currentTime, accumulator);
	}
}

} /* namespace Graphic */
} /* namespace story */
