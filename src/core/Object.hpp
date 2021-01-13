#pragma once

#include "graphic/CoreObject.hpp"
#include "graphic/movement/Movement.hpp"

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
  void addMovement(Graphic::Movement::Movement movement) {
    this->movement = std::make_shared<Graphic::Movement::Movement>(movement);
  }

  double getX() { return graphicObject->getX(); }
  double getY() { return graphicObject->getY(); }
  void setX(double x) { graphicObject->setX(x); }
  void setY(double y) { graphicObject->setY(y); }
  double getWidth() { return graphicObject->getWidth(); }
  double getHeight() { return graphicObject->getHeight(); }
  void setWidth(double width, bool ratio) { graphicObject->setWidth(width, ratio); }
  void setHeight(double height, bool ratio) { graphicObject->setHeight(height, ratio); }
  void moveTo(double x, double y) { graphicObject->moveTo(x, y); }
  void moveBy(double x, double y) { graphicObject->moveBy(x, y); }

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
  virtual void render();

private:
  std::shared_ptr<Graphic::CoreObject> graphicObject;
  std::shared_ptr<Graphic::Movement::Movement> movement;
};

} /* namespace Core */
} /* namespace story */
