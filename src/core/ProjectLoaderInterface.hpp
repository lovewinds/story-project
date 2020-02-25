#pragma once

#include <string>

namespace story {
namespace Resource {
  class ResourceManager;
}
}

namespace story {
namespace Core {

class ProjectLoaderInterface
{
public:
  ProjectLoaderInterface(story::Resource::ResourceManager* manager) {
    this->resManager = manager;
  }
  virtual ~ProjectLoaderInterface(){ }

  /* Load all resources information from resource archive */
  virtual bool loadProject(std::string& res_path) = 0;

protected:
  story::Resource::ResourceManager *resManager;
};

} /* namespace Core */
} /* namespace story */
