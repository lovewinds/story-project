#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <memory>

#include "graphic/layer/Layer.hpp"
#include "graphic/descriptors/SceneDescripor.hpp"
#include "graphic/descriptors/SpriteDescriptor.hpp"
#include "resource/EImageResource.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "util/SDLWrap.hpp"

namespace story {
namespace Core {
  class ProjectLoaderInterface;
  class XMLProjectLoader;
}
}

namespace story {
namespace Resource {

class EResourceManager
{
public:
  EResourceManager();
  ~EResourceManager();

  /* Prohibit copy constructor */
  EResourceManager(EResourceManager& self) = delete;

  /* Load all resources information from resource archive */
  bool loadProject(std::string res_file);

/*
 * SceneDesc Functions
 */
  /* Create scene descriptor with given parameters */
  bool addSceneDesc(std::string name,
    std::shared_ptr<story::Graphic::ESceneDesc> desc);

  std::shared_ptr<story::Graphic::ESceneDesc>
  getSceneDesc(std::string name);

  /* Create scene with name
   * (for some debug purpose) */
  std::shared_ptr<story::Graphic::Layer>
  createScene(story::Graphic::LayerType type, std::string scene_name);

/*
 * Image Resource Functions
 */
  /* ResourceManager holds Image resource to handle its repetitive usage.
   * uses 'name' as key,
   * uses 'path' to load real image file.
   */
  std::shared_ptr<EImageResource>
  createImageResource(std::string name, std::string path,
    unsigned int width, unsigned int height);

  /* Returns shared_ptr of ImageResource instance allocated */
  std::shared_ptr<EImageResource>
  getImageResource(std::string resource_name);

  void updateImageResourceCache();

  std::shared_ptr<story::Graphic::ImageTexture>
  createImageTexture(std::string name, std::string base_image);

/*
 * Sprite Functions
 */
  bool createSpriteDesc(std::shared_ptr<Graphic::ESpriteDesc>);

  std::shared_ptr<Graphic::ESpriteDesc>
  getSpriteDesc(std::string desc_name);

  std::shared_ptr<story::Graphic::SpriteTexture>
  createSprite(std::string type, std::string name);

/*
 * Texture Functions
 */
  std::shared_ptr<SDLTextureWrap> allocateTexture(std::string path);
  void releaseTexture(std::string path);

/*
 * TextTexture
 */
  std::shared_ptr<SDLFontWrap> getFont(std::string family, int size);
  void releaseFont(std::shared_ptr<SDLFontWrap>& font);

protected:
  story::Core::ProjectLoaderInterface *loader;

  /* Store sprite info here for caching */
  std::map<std::string, std::shared_ptr<Graphic::ESpriteDesc>> sprite_desc_dict;

  std::map<std::string, std::shared_ptr<story::Graphic::ESceneDesc>> scene_desc_map;

  /* image map uses path as a key, it can have simillar prefix.
   * unordered_map can be more effecient for this case */
  std::unordered_map<std::string, std::shared_ptr<EImageResource>> image_map;
};

} /* namespace Resource */
} /* namespace story */
