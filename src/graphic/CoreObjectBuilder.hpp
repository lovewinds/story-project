#pragma once

#include <memory>

#include "core/Ecore.hpp"
#include "graphic/CoreObject.hpp"
#include "resource/ResourceManager.hpp"

namespace story {
namespace Graphic {

class CoreObjectBuilder {
public:
  CoreObjectBuilder();
  virtual ~CoreObjectBuilder();

  std::shared_ptr<Graphic::CoreObject> build(std::string textureName) {
    story::Resource::ResourceManager& resManager =
      story::Core::Ecore::getInstance()->getResourceManager();

    auto texture = resManager.createImageTexture("uuid", textureName);

    std::shared_ptr<Graphic::CoreObject> obj(new Graphic::CoreObject());
    obj->addTexture(texture);

    return obj;
  }

};

} /* namespace Graphic */
} /* namespace story */
