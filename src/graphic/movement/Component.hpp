#pragma once

#include <functional>

namespace story {
namespace Graphic {
namespace Movement {

enum Config
{
  ONCE,
  REPEAT,
};

struct Position {
  Position() : x(0.0), y(0.0), angle(0.0) {};
  Position(double x, double y, double angle) : x(x), y(y), angle(angle) {}
  double x;
  double y;
  double angle;
};

class Component {
public:
  Component() {};
  virtual ~Component() {};

  virtual Position update(uint32_t currentTime, uint32_t accumulator = 0) {
    Position p;
    if (func) p = func(currentTime, accumulator);
    return p;
  }

protected:
  uint32_t preDelayTime = 0;
  uint32_t postDelayTime = 0;
  uint32_t motionTime = 0;

  // Movement function
  std::function<Position (uint32_t, uint32_t)> func;
};

} // namespace Movement
} // namespace Graphic
} // namespace story
