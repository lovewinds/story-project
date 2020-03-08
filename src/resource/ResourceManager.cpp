#include <SDL.h>

#include <string>
#include <fstream>

#include "core/Ecore.hpp"
#include "core/ProjectLoaderInterface.hpp"
#include "core/XMLProjectLoader.hpp"
#include "util/LogHelper.hpp"
#include "resource/ResourceManager.hpp"

#include "screen/layer/RPGLayer.hpp"
#include "screen/layer/MapLayer.hpp"
#include "screen/layer/ChatLayer.hpp"
#include "screen/layer/DbgOverlayLayer.hpp"
#include "screen/layer/TitleLayer.hpp"

namespace story {
namespace Resource {

ResourceManager::ResourceManager()
{
  /* TODO: Get specific loader from factory class */
  loader = new story::Core::XMLProjectLoader(this);
  SDL_assert(loader);
}

ResourceManager::~ResourceManager()
{
  if (loader) {
    delete loader;
    loader = NULL;
  }

  scene_desc_map.clear();
  image_map.clear();

  LOG_DBG("Bye ResourceManager !");
}

/***********************************
 * Scene Functions
 ***********************************/
bool ResourceManager::addSceneDesc(std::string name,
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
ResourceManager::getSceneDesc(std::string name)
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

std::shared_ptr<Screen::ScreenLayer>
ResourceManager::createScene(story::Graphic::LayerType type, std::string scene_name)
{
  LOG_DBG("Trying to create scene [%s]", scene_name.c_str());
  //std::shared_ptr<Layer> scene(new Layer(scene_name));
  std::shared_ptr<Screen::ScreenLayer> scene;

  switch (type)
  {
  case story::Graphic::LAYER_RPG:
    scene = std::shared_ptr<Screen::ScreenLayer>(new Screen::RPGLayer(scene_name));
    break;
  case story::Graphic::LAYER_CHAT:
    scene = std::shared_ptr<Screen::ScreenLayer>(new Screen::ChatLayer(scene_name));
    break;
  case story::Graphic::LAYER_DEBUG:
    scene = std::shared_ptr<Screen::ScreenLayer>(new Screen::DbgOverlayLayer(scene_name));
    break;
  case story::Graphic::LAYER_TITLE:
    scene = std::shared_ptr<Screen::ScreenLayer>(new Screen::TitleLayer(scene_name));
    break;
  case story::Graphic::LAYER_MAP:
    scene = std::shared_ptr<Screen::ScreenLayer>(new Screen::MapLayer(scene_name));
    break;
  default:
    /* TODO: If new scene type added, should be created here */
    return nullptr;
    break;
  }

  return scene;
}

std::shared_ptr<Screen::ScreenLayer>
ResourceManager::createScene(std::string scene_name)
{
  std::shared_ptr<Screen::ScreenLayer> scene = nullptr;

  /* Search scene desc */
  std::shared_ptr<story::Graphic::ESceneDesc> sceneDesc = getSceneDesc(scene_name);
  if (nullptr == sceneDesc) {
    LOG_ERR("Failed to find scene descriptor");
    return nullptr;
  }

  LOG_DBG("Creating scene [%s].....", sceneDesc->getName().c_str());
  scene = createScene(sceneDesc->getType(), scene_name);

  /* For each layer */
  for (auto& layer_iter : sceneDesc->layer_list)
  {
    std::shared_ptr<story::Graphic::SubLayerDesc> layer = layer_iter;
    LOG_DBG("  Scene Layer  [%s]", layer->getName().c_str());

    /* Create sprites from sprite descriptor */
    for (auto& obj_desc : layer->obj_array)
    {
      std::shared_ptr<Resource::ProjectObject> object = obj_desc;
      LOG_DBG("    Object descriptor [%s]::[%s] (%03d, %03d)",
        object->get("name").c_str(),
        object->get("tag_type").c_str(),
        object->getInt("x"), object->getInt("y")
      );
    }

    /* Create sprites from sprite descriptor */
    for (auto& sprite_it : layer->sprite_list)
    {
      std::shared_ptr<story::Graphic::ESpriteDesc> spriteDesc = sprite_it;
      std::shared_ptr<story::Graphic::SpriteTexture> sprite;
      std::shared_ptr<Graphic::Object> object
        (new story::Graphic::Object());
      LOG_DBG("    Sprite [%s] (%03d, %03d)",
        spriteDesc->getName().c_str(),
        spriteDesc->getX(), spriteDesc->getY()
      );

      /* TODO: Creation should be performed with ObjectFactory */
      sprite = createSprite(spriteDesc);
      object->setName(spriteDesc->getName());
      object->movePositionTo(spriteDesc->getX(), spriteDesc->getY());
      if (spriteDesc->isControllable())
        object->setControllable(true);

      object->addSprite(sprite);
      scene->addObject(object);
      //scene->addSprite(sprite);
    }

    /* Create images from image descriptor */
    for (auto& image_it : layer->image_list)
    {
      std::shared_ptr<story::Graphic::EImageDesc> imageDesc = image_it;
      std::shared_ptr<story::Graphic::ImageTexture> image;
      std::shared_ptr<story::Graphic::Object> object
        (new story::Graphic::Object());
      LOG_DBG("    Image [%s]", imageDesc->getName().c_str());

      object->setName(imageDesc->getName());
      image = createImageTexture(imageDesc);
      if (nullptr == image) {
        LOG_ERR("Failed to create Image !");
      } else {
        // auto h_align = imageDesc->getHorizontalAlign();
        // auto v_align = imageDesc->getVerticalAlign();
        int px = imageDesc->getX();
        int py = imageDesc->getY();
        double width = 0.0f;
        double height = 0.0f;

        /* TODO: Inefficient logic */
        if (imageDesc->getWidthRatio() != 0.0) {
          image->setWidth(imageDesc->getWidthRatio(), true);
          LOG_ERR("Width [%d] / Ratio [%lf]", image->getWidth(), imageDesc->getWidthRatio());
          width = (imageDesc->getWidthRatio() / 100.0) * image->getWidth();
          LOG_ERR("Calculated Width [%lf]", width);
        }
        else {
          image->setWidth(imageDesc->getWidth(), false);
          width = image->getWidth();
        }
        if (imageDesc->getHeightRatio() != 0.0) {
          image->setHeight(imageDesc->getHeightRatio(), true);
          height = (imageDesc->getHeightRatio() / 100.0) * image->getHeight();
        }
        else {
          image->setHeight(imageDesc->getHeight(), false);
          height = image->getHeight();
        }

        if (imageDesc->getHorizontalAlign() == story::Graphic::IMAGE_ALIGN_RIGHT) {
          px = (story::Core::Ecore::getScreenWidth() - (int)width) - px;
        }
        if (imageDesc->getVerticalAlign() == story::Graphic::IMAGE_ALIGN_BOTTOM) {
          py = (story::Core::Ecore::getScreenHeight() - (int)height) + py;
        }

        object->movePositionTo(px, py);

        object->addImage(image);
        scene->addObject(object);
        //scene->addImage(image);
      }
    }

    /* Create images from image descriptor */
    for (auto& grid_it : layer->grid_list)
    {
      std::shared_ptr<story::Graphic::EGridDesc> gridDesc = grid_it;
      std::string base_tile_image = "MapTile";
      base_tile_image = gridDesc->getBaseImage();

      /* RECOMMEND: Do not use dynamic_cast */
      Screen::RPGLayer* rpg_scene = dynamic_cast<Screen::RPGLayer*>(scene.get());
      Screen::MapLayer* map_scene = dynamic_cast<Screen::MapLayer*>(scene.get());
      std::shared_ptr<story::Graphic::GridMapTexture> map(
        new story::Graphic::GridMapTexture("MyMap", base_tile_image.c_str(), gridDesc)
      );
      map->setTileSize(gridDesc->getGridWidth(), gridDesc->getGridHeight());
      if (rpg_scene) {
        rpg_scene->setMap(map);
        rpg_scene->setGridDescriptor(gridDesc);
      }
      else if (map_scene) {
        map_scene->setMap(map);
        map_scene->setGridDescriptor(gridDesc);
      }
    }
  }

  return scene;
}

void ResourceManager::removeScene(std::string scene_name)
{
  /* TODO: Notify removal to resource manager to handle ref count */
}

/***********************************
 * Image texture Functions
 ***********************************/
std::shared_ptr<EImageResource>
ResourceManager::createImageResource(std::string name, std::string path,
    unsigned int width, unsigned int height)
{
  std::shared_ptr<EImageResource> imgInfo(new EImageResource(name, path, width, height));
  if (nullptr == imgInfo) {
    LOG_ERR("Failed to create Image Resource info!!");
    return nullptr;
  }

  //std::pair<std::map<std::string, std::shared_ptr<ESceneInfo>>::iterator, bool> result;
  auto map_result = image_map.emplace(name, imgInfo);
  if (false == map_result.second) {
    LOG_ERR("Resource created, but failed to insert image resource map!");
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
ResourceManager::getImageResource(std::string resource_name)
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

void ResourceManager::updateImageResourceCache()
{
  LOG_DBG("Count of allocated image resources: [%lu]", image_map.size());
  if (image_map.empty())
    return;

  for (auto it = image_map.begin(); it != image_map.end(); it++) {
    LOG_INFO("  ImageResource[%s]: [%lu]",
      it->first.c_str(), it->second.use_count());

    /* Currently, SpriteTexture and ImageTexture refer ImageResource.
     * If there is NO reference, deallocate texture */
    if (it->second.unique())
      it->second->releaseTexture();
  }
}

std::shared_ptr<story::Graphic::ImageTexture>
ResourceManager::createImageTexture(std::string name, std::string base_image)
{
  std::shared_ptr<story::Graphic::ImageTexture> imgTexture(
    new story::Graphic::ImageTexture(name, base_image)
  );
  if (!imgTexture) {
    LOG_ERR("Failed to create Image texture !!");
    return nullptr;
  }
  return imgTexture;
}

std::shared_ptr<story::Graphic::ImageTexture>
ResourceManager::createImageTexture(std::shared_ptr<story::Graphic::EImageDesc> imageDesc)
{
  /* Search image desc */
  std::shared_ptr<story::Graphic::ImageTexture> image = nullptr;

  if (nullptr == imageDesc)
    return nullptr;

  image = createImageTexture(imageDesc->getName(), imageDesc->getType());

  return image;
}

void ResourceManager::removeImageTexture(std::string name)
{

}

/***********************************
 * Sprite Functions
 ***********************************/
bool ResourceManager::createSpriteDesc(std::shared_ptr<Graphic::ESpriteDesc> spriteDesc)
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
ResourceManager::getSpriteDesc(std::string type_name)
{
  std::shared_ptr<Graphic::ESpriteDesc> found = nullptr;
  auto spriteDesc = sprite_desc_dict.find(type_name);

  if (spriteDesc != sprite_desc_dict.end()) {
    /* Found */
    found = spriteDesc->second;
  }

  return found;
}

std::shared_ptr<story::Graphic::SpriteTexture>
ResourceManager::createSprite(std::string name, std::string type)
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
  std::shared_ptr<story::Graphic::SpriteTexture> sprite(
    new story::Graphic::SpriteTexture(name)
  );

  if (false == sprite->initialize(spriteDesc)) {
    LOG_ERR("Failed to initialize sprite! [Type:%s]", spriteDesc->getName().c_str());
    return nullptr;
  }

  return sprite;
}

std::shared_ptr<story::Graphic::SpriteTexture>
ResourceManager::createSprite(std::shared_ptr<story::Graphic::ESpriteDesc> spriteDesc)
{
  std::shared_ptr<story::Graphic::SpriteTexture> sprite = nullptr;

  if (nullptr == spriteDesc)
    return nullptr;

  sprite = createSprite(
    spriteDesc->getObjectName(),
    spriteDesc->getName()
  );

  return sprite;
}

void ResourceManager::removeSprite(std::string name)
{

}

/*
 * Low layer functions
 */
std::shared_ptr<SDLTextureWrap> ResourceManager::allocateTexture(std::string path)
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

void ResourceManager::releaseTexture(std::string path)
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

std::shared_ptr<SDLFontWrap> ResourceManager::getFont(std::string family, int size)
{
  std::shared_ptr<SDLFontWrap> font = story::Core::Ecore::loadFont(family, size);

  /* TODO: Caching */
  return font;
}

void ResourceManager::releaseFont(std::shared_ptr<SDLFontWrap>& font)
{
  /* TODO: Unref cached font */
}

std::shared_ptr<SDLTextureWrap>
ResourceManager::createTextTexture(std::string text,
    SDL_Color textColor, SDL_Color bgColor,
    std::shared_ptr<SDLFontWrap> font)
{
  /* If not provided, create a default one */
  if (nullptr == font) {
    font = getFont("NanumGothic", 28);
    if (nullptr == font) {
      LOG_ERR("Failed to get font[%s] size[%d]", "NanumGothic", 28);
      return nullptr;
    }
  }

  /* Currently, text textures are not cached */
  std::shared_ptr<SDLTextureWrap> texture(
    new SDLTextureWrap(text, textColor, bgColor, font));

  LOG_DBG("[ResFactory] texture [%p]", &texture);
  return texture;
}

/***********************************
 * Project
 ***********************************/
bool ResourceManager::loadProject(std::string res_file)
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

} /* namespace Resource */
} /* namespace story */
