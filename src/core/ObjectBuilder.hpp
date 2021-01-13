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
  ObjectBuilder() { }
  virtual ~ObjectBuilder() { }

  void setTexture(std::string textureName) {
    this->textureName = textureName;
  }

  std::shared_ptr<Core::Object> build() {
    std::shared_ptr<Graphic::CoreObjectBuilder> cob(
      new Graphic::CoreObjectBuilder()
    );
    auto o = cob->build(textureName);

    std::shared_ptr<Core::Object> obj(
      new Core::Object(o)
    );
    return obj;
  }

private:
  std::string textureName;
};

} /* namespace Core */
} /* namespace story */
