#pragma once

#include "graphic/CoreObject.hpp"

namespace story {
namespace Core {

class Object {
public:
  Object();
  virtual ~Object();

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
  virtual void render();

  void addGraphicObject(std::shared_ptr<Graphic::CoreObject> graphicObject);

private:
  double p_x;
  double p_y;
};

} /* namespace Graphic */
} /* namespace story */
