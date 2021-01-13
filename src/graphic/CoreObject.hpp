#pragma once

#include "graphic/texture/Texture.hpp"

namespace story {
namespace Graphic {

class CoreObject {
public:
  CoreObject() {}
  virtual ~CoreObject() {}

  void setTexture(std::shared_ptr<Graphic::Texture> texture) {
    this->texture = texture;
  }

  double getX() { return p_x; }
  double getY() { return p_y; }
  void setX(double x) { p_x = x; }
  void setY(double y) { p_y = y; }
  double getWidth() { return width; }
  double getHeight() { return height; }
  void setWidth(double width, bool ratio) {
    this->width = width;
    texture->setWidth(width, ratio);
  }
  void setHeight(double height, bool ratio) {
    this->height = height;
    texture->setHeight(height, ratio);
  }

  void moveTo(double x, double y) {
    p_x = x;
    p_y = y;
  }
  void moveBy(double x, double y) {
    p_x += x;
    p_y += y;
  }

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0) {
    texture->update(currentTime, accumulator);
  }
  virtual void render(double x = 0.0, double y = 0.0) {
    texture->render(p_x + x, p_y + y, 0.0, 0.0, 0.0);
  }

private:
  double p_x;
  double p_y;
  double width;
  double height;
  std::shared_ptr<Graphic::Texture> texture;
};

} /* namespace Graphic */
} /* namespace story */
