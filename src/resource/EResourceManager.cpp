#include <SDL.h>

#include <string>
#include <fstream>

#include "core/Ecore.hpp"
#include "core/ProjectLoaderInterface.hpp"
#include "core/XMLProjectLoader.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"

#include "graphic/layer/RPGLayer.hpp"
#include "graphic/layer/MapLayer.hpp"
#include "graphic/layer/ChatLayer.hpp"
#include "graphic/layer/DbgOverlayLayer.hpp"
#include "graphic/layer/TitleLayer.hpp"

namespace story {
namespace Resource {

EResourceManager::EResourceManager()
{
  /* TODO: Get specific loader from factory class */
  loader = new story::Core::XMLProjectLoader(this);
  SDL_assert(loader);
}

EResourceManager::~EResourceManager()
{
  if (loader) {
    delete loader;
    loader = NULL;
  }

  scene_desc_map.clear();
  image_map.clear();

  LOG_DBG("Bye ResourceManager !");
}

bool EResourceManager::loadProject(std::string res_file)
{
  std::string path = story::Core::Ecore::getResourcePath(res_file);
  if (path.empty()) {
    LOG_ERR("Empty resource file path !");
    return false;
  }
  LOG_DBG("Load resource file [%s]", path.c_str());

  if (story::Core::Ecore::checkPlatform("iOS")) {
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

  return loader->loadProject(path);
}

bool EResourceManager::addSceneDesc(std::string name,
  std::shared_ptr<story::Graphic::ESceneDesc> desc)
{
  auto result = scene_desc_map.emplace(name, desc);
  if (!result.second) {
    LOG_ERR("Failed to insert scene descriptor !");
    return false;
  }

  return true;
}

std::shared_ptr<story::Graphic::ESceneDesc>
EResourceManager::getSceneDesc(std::string name)
{
  std::shared_ptr<story::Graphic::ESceneDesc> result = nullptr;
  auto sceneDesc = scene_desc_map.find(name);

  if (sceneDesc != scene_desc_map.end()) {
    result = sceneDesc->second;
  } else {
    result = nullptr;
  }

  return result;
}

std::shared_ptr<story::Graphic::Layer>
EResourceManager::createScene(story::Graphic::LayerType type, std::string scene_name)
{
  LOG_DBG("Trying to create scene [%s]", scene_name.c_str());
  //std::shared_ptr<Layer> scene(new Layer(scene_name));
  std::shared_ptr<story::Graphic::Layer> scene;

  switch (type)
  {
  case story::Graphic::LAYER_RPG:
    scene = std::shared_ptr<story::Graphic::Layer>(new story::Graphic::RPGLayer(scene_name));
    break;
  case story::Graphic::LAYER_CHAT:
    scene = std::shared_ptr<story::Graphic::Layer>(new story::Graphic::ChatLayer(scene_name));
    break;
  case story::Graphic::LAYER_DEBUG:
    scene = std::shared_ptr<story::Graphic::Layer>(new story::Graphic::DbgOverlayLayer(scene_name));
    break;
  case story::Graphic::LAYER_TITLE:
    scene = std::shared_ptr<story::Graphic::Layer>(new story::Graphic::TitleLayer(scene_name));
    break;
  case story::Graphic::LAYER_MAP:
    scene = std::shared_ptr<story::Graphic::Layer>(new story::Graphic::MapLayer(scene_name));
    break;
  default:
    /* TODO: If new scene type added, should be created here */
    return nullptr;
    break;
  }

  return scene;
}

std::shared_ptr<SDLTextureWrap> EResourceManager::allocateTexture(std::string path)
{
  std::shared_ptr<SDLTextureWrap> texture(new SDLTextureWrap(path));

  LOG_INFO("Creating raw texture for [%s]", path.c_str());
  if (nullptr == texture) {
    LOG_ERR("Failed to allocate texture !");
  }

#if 0
  std::shared_ptr<SDLTextureWrap> result;
  auto found = _texture_map.find(path);

  if (found == _texture_map.end()) {
    /* Texture is not exist. allocate one */
    std::shared_ptr<SDLTextureWrap> texture(new SDLTextureWrap(path));
    if (texture) {
      //auto insert = _texture_map.emplace(path, texture);
      std::pair<std::string, std::shared_ptr<SDLTextureWrap>> p(path, texture);
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
#endif
  return texture;
}

void EResourceManager::releaseTexture(std::string path)
{
#if 0
  LOG_ERR("  Count of texture map items [%lu]", _texture_map.size());
  if (_texture_map.empty())
    return;

  std::shared_ptr<SDLTextureWrap> result;
  auto found = _texture_map.find(path);
  if (found != _texture_map.end()) {
    LOG_DBG("  texture [%s] count [%lu]", found->first.c_str(), found->second.use_count());
    /* Reference counting logic
     * If use_count is 1, it is not owned by all activated scenes.
     * remove it ! */
    if (found->second.use_count() == 1)
      _texture_map.erase(found);
  }
#endif
}

bool EResourceManager::createSpriteDesc(std::shared_ptr<Graphic::ESpriteDesc> spriteDesc)
{
  if (!spriteDesc)
    return false;

  auto inserted = sprite_desc_dict.emplace(spriteDesc->getName(), spriteDesc);
  if (!inserted.second) {
    LOG_ERR("Failed to insert sprite descriptor map!");
    return false;
  }

  return true;
}

std::shared_ptr<Graphic::ESpriteDesc>
EResourceManager::getSpriteDesc(std::string type_name)
{
  std::shared_ptr<Graphic::ESpriteDesc> found = nullptr;
  auto spriteDesc = sprite_desc_dict.find(type_name);

  if (spriteDesc != sprite_desc_dict.end()) {
    /* Found */
    found = spriteDesc->second;
  }

  return found;
}

std::shared_ptr<story::Graphic::ESprite>
EResourceManager::createSprite(std::string name, std::string type)
{
  LOG_DBG("Trying to create sprite type [%s] / name [%s]", type.c_str(), name.c_str());

  /* Find sprite template type */
  auto t = sprite_desc_dict.find(type);
  if (t == sprite_desc_dict.end()) {
    LOG_ERR("Sprite descriptor [%s] is not defined.", type.c_str());
    return nullptr;
  }

  /* Create sprite from descriptor */
  std::shared_ptr<Graphic::ESpriteDesc> spriteDesc = t->second;
  std::shared_ptr<story::Graphic::ESprite> sprite(
    new story::Graphic::ESprite(name)
  );

  if (false == sprite->initialize(spriteDesc)) {
    LOG_ERR("Failed to initialize sprite! [Type:%s]", spriteDesc->getName().c_str());
    return nullptr;
  }

  return sprite;
}

std::shared_ptr<EImageResource>
EResourceManager::createImageResource(std::string name, std::string path,
    unsigned int width, unsigned int height)
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
#if 0
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
  //std::weak_ptr<EImageResource> imgRes = image_map.find(resource_name);
  std::shared_ptr<EImageResource> found;
  auto imgRes = image_map.find(resource_name);

  if (imgRes != image_map.end()) {
    /* ImageResource found */
    found = imgRes->second;
  } else {
    found = nullptr;
  }

  /* not found */
  return found;
}

void EResourceManager::updateImageResourceCache()
{
  LOG_DBG("Count of allocated image resources: [%lu]", image_map.size());
  if (image_map.empty())
    return;

  for (auto it = image_map.begin(); it != image_map.end(); it++) {
    LOG_INFO("  ImageResource[%s]: [%lu]",
      it->first.c_str(), it->second.use_count());

    /* Currently, ESprite and EImageTexture refer ImageResource.
     * If there is NO reference, deallocate texture */
    if (it->second.unique())
      it->second->releaseTexture();
  }
}

std::shared_ptr<story::Graphic::EImageTexture>
EResourceManager::createImageTexture(std::string name, std::string base_image)
{
  std::shared_ptr<story::Graphic::EImageTexture> imgTexture(
    new story::Graphic::EImageTexture(name, base_image)
  );
  if (!imgTexture) {
    LOG_ERR("Failed to create Image texture !!");
    return nullptr;
  }
  return imgTexture;
}

std::shared_ptr<SDLFontWrap> EResourceManager::getFont(std::string family, int size)
{
  std::shared_ptr<SDLFontWrap> font = story::Core::Ecore::loadFont(family, size);

  /* TODO: Caching */
  return font;
}

void EResourceManager::releaseFont(std::shared_ptr<SDLFontWrap>& font)
{
  /* TODO: Unref cached font */
}

} /* namespace Resource */
} /* namespace story */
