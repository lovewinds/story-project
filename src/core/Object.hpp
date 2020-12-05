#pragma once

#include "graphic/CoreObject.hpp"

namespace story {
namespace Core {

class Object {
public:
  Object(std::shared_ptr<Graphic::CoreObject> graphicObject) {
    this->graphicObject = graphicObject;
  }
  virtual ~Object() { }

  void addGraphicObject(std::shared_ptr<Graphic::CoreObject> graphicObject) {
    this->graphicObject = graphicObject;
  }

  double getX() { return graphicObject->getX(); }
  double getY() { return graphicObject->getY(); }
  void setX(double x) { graphicObject->setX(x); }
  void setY(double y) { graphicObject->setY(y); }
  void moveTo(double x, double y) { graphicObject->moveTo(x, y); }
  void moveBy(double x, double y) { graphicObject->moveBy(x, y); }

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0) {
    graphicObject->update(currentTime, accumulator);
  }
  virtual void render() {
    graphicObject->render();
  }

private:
  std::shared_ptr<Graphic::CoreObject> graphicObject;
};

} /* namespace Core */
} /* namespace story */
