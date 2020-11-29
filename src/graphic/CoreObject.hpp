#pragma once

#include "graphic/texture/Texture.hpp"

namespace story {
namespace Graphic {

class CoreObject {
public:
  CoreObject();
  virtual ~CoreObject();

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
  virtual void render();

  void addTexture(std::shared_ptr<Graphic::Texture> texture);
};

} /* namespace Graphic */
} /* namespace story */
