#pragma once

#include <memory>

#include "core/Ecore.hpp"
#include "core/Object.hpp"
#include "graphic/CoreObjectBuilder.hpp"
#include "resource/ResourceManager.hpp"

namespace story {
namespace Core {

class ObjectBuilder {
public:
  ObjectBuilder();
  virtual ~ObjectBuilder();

  std::shared_ptr<Core::Object> build(std::string textureName) {
    std::shared_ptr<Graphic::CoreObjectBuilder> cob(
      new Graphic::CoreObjectBuilder()
    );
    auto o = cob->build(textureName);

    std::shared_ptr<Core::Object> obj(new Core::Object());
    obj->addGraphicObject(o);

    return obj;
  }

};

} /* namespace Core */
} /* namespace story */
