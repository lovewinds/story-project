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

  void addGraphicObject(std::shared_ptr<Graphic::CoreObject> graphicObject) {
    this->graphicObject = graphicObject;
  }

  double getX() { return p_x; }
  double getY() { return p_y; }
  void setX(double x) { p_x = x; }
  void setY(double y) { p_y = y; }

  void moveTo(double x, double y) {
    p_x = x;
    p_y = y;
  }
  void moveBy(double x, double y) {
    p_x += x;
    p_y += y;
  }

private:
  double p_x;
  double p_y;
  std::shared_ptr<Graphic::CoreObject> graphicObject;
};

} /* namespace Graphic */
} /* namespace story */
