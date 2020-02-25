#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/ResourceManager.hpp"
#include "graphic/texture/SpriteTexture.hpp"

#include "screen/layer/ScreenLayer.hpp"

namespace story {
namespace Screen {

ScreenLayer::ScreenLayer()
  : sprite_map(_sprite_map)
{
  LOG_INFO("Superclass");

  isActivated = true;
}

ScreenLayer::ScreenLayer(std::string name)
: sprite_map(_sprite_map)
{
  LOG_INFO("Layer[%s] created", name.c_str());
  this->name = name;

  isActivated = true;
}

ScreenLayer::~ScreenLayer()
{
  _sprite_map.clear();
  _text_texture_map.clear();
  _object_map.clear();

  isActivated = false;

  LOG_INFO("Layer[%s] removed", name.c_str());
}

std::string ScreenLayer::getName()
{
  return name;
}

bool ScreenLayer::addObject(std::shared_ptr<Graphic::Object> object)
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
bool ScreenLayer::addSprite(std::shared_ptr<SpriteTexture> sprite)
{
  auto result = _sprite_map.emplace(sprite->getName(), sprite);
  if (!result.second) {
    LOG_ERR("Failed to insert sprite map!");
    return false;
  }
  return true;
}

bool ScreenLayer::addImage(std::shared_ptr<ImageTexture> imgTexture)
{
  auto result = _img_texture_map.emplace(imgTexture->getName(), imgTexture);
  if (!result.second) {
    LOG_ERR("Failed to insert image map!");
    return false;
  }
  return true;
}
#endif

void ScreenLayer::setActiveState(bool active)
{
  isActivated = active;
}

void ScreenLayer::handleEvent(SDL_Event e)
{
  
}

void ScreenLayer::render()
{
  for (auto &it : _img_texture_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _sprite_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _text_texture_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _object_map)
  {
    it.second->render();
  }

  for (auto& it : _raw_texture_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }
}

void ScreenLayer::update(Uint32 currentTime, Uint32 accumulator)
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
