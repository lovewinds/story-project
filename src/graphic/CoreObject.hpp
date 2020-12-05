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
  virtual void render() {
    texture->render(p_x, p_y, 0.0, 0.0, 0.0);
  }

private:
  double p_x;
  double p_y;
  std::shared_ptr<Graphic::Texture> texture;
};

} /* namespace Graphic */
} /* namespace story */
