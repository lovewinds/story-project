#pragma once

#include <string>

#include <pugixml.hpp>

#include "core/ProjectLoaderInterface.hpp"
#include "graphic/descriptors/SceneDescripor.hpp"
#include "resource/ProjectObject.hpp"

namespace story {
namespace Resource {
  class ResourceManager;
}
}

namespace story {
namespace Core {

class XMLProjectLoader : public ProjectLoaderInterface
{
public:
  XMLProjectLoader(story::Resource::ResourceManager* mgr);
  virtual ~XMLProjectLoader();

  /* Load all resources information from resource archive */
  virtual bool loadProject(std::string& res_path);

private:
  void loadSprites(pugi::xml_document &document);
  void loadCommonResources(pugi::xml_document &document);
  void loadScenes(pugi::xml_document &document);

  void loadProjectScene(pugi::xml_document &document);
  void loadProjectSceneLayer(pugi::xml_document &document);

  void loadProjectObject(pugi::xml_document &document);

  std::vector<std::shared_ptr<Resource::ProjectObject>>
  createSceneObject(pugi::xml_node &node);

  std::shared_ptr<story::Graphic::ESceneDesc> loadSceneDesc(
    pugi::xml_document &document,
    std::string scene_name, story::Graphic::LayerType scene_type);
};

} /* namespace Core */
} /* namespace story */
