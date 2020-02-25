#include <SDL_image.h>

#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/ResourceManager.hpp"
#include "resource/EImageResource.hpp"

namespace story {
namespace Resource {

EImageResource::EImageResource(
    std::string name, std::string path,
    unsigned int width, unsigned int height)
{
  this->name = name;
  this->path = story::Core::Ecore::getResourcePath(path);
  this->width = width;
  this->height = height;
  LOG_DBG("EImageResource[%16s] :: %s", this->name.c_str(), this->path.c_str());

  /* TODO: Don't allocate on creation */
  //allocate();
}

EImageResource::~EImageResource()
{
  releaseTexture();
  LOG_ERR("EImageResource[%s] removed", name.c_str());
}

std::string EImageResource::getName() const
{
  return name;
}

std::string EImageResource::getPath() const
{
  return path;
}

unsigned int EImageResource::getWidth() const
{
  return width;
}

unsigned int EImageResource::getHeight() const
{
  return height;
}

std::shared_ptr<SDLTextureWrap> EImageResource::getTexture()
{
  if (texture == nullptr) {
    ResourceManager& resManager = story::Core::Ecore::getInstance()->getResourceManager();

    LOG_ERR("Texture is not exists. create one");
    texture = resManager.allocateTexture(path);
    if (!texture) {
      LOG_ERR("Failed to allocate resource [%s]", getPath().c_str());
      return nullptr;
    }
  }

  return texture;
}

void EImageResource::releaseTexture()
{
  texture.reset();
  texture = nullptr;

  ResourceManager& resManager = story::Core::Ecore::getInstance()->getResourceManager();
  resManager.releaseTexture(path);
}

} /* namespace Resource */
} /* namespace story */
