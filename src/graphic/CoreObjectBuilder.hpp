#pragma once

#include <memory>

#include "core/Ecore.hpp"
#include "graphic/CoreObject.hpp"
#include "resource/ResourceManager.hpp"

namespace story {
namespace Graphic {

class CoreObjectBuilder {
public:
  CoreObjectBuilder() { }
  virtual ~CoreObjectBuilder() { }

  std::shared_ptr<CoreObject> build(std::string textureName) {
    Resource::ResourceManager& resManager =
      Core::Ecore::getInstance()->getResourceManager();
    auto texture = resManager.createImageTexture("uuid", textureName);

    std::shared_ptr<CoreObject> obj(
      new CoreObject()
    );
    obj->setTexture(texture);

    return obj;
  }

};

} /* namespace Graphic */
} /* namespace story */
