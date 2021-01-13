#pragma once

#include <iostream>
#include <memory>
#include "graphic/movement/Component.hpp"

namespace story {
namespace Graphic {
namespace Movement {

enum MovementState
{
  MOVEMENT_NONE,
  MOVEMENT_STOP,
  MOVEMENT_START,
  MOVEMENT_PAUSE,
  MOVEMENT_RESUME,
};

enum MovementConfig
{
  CONFIG_ONCE,
  CONFIG_REPEAT,
};

class Movement {
public:
  Movement();
  Movement(double x, double y, double angle);
  virtual ~Movement() {};

  virtual void start();
  virtual void stop();
  virtual void pause();
  virtual void resume();
  virtual void sync();

  double getX() { return x; }
  double getY() { return y; }
  double getAngle() { return angle; }

  virtual void update(unsigned int currentTime, unsigned int accumulator = 0);
  void setComponent(std::shared_ptr<Component> c) {
    component = c;
  }

  Movement& operator+ (Movement& ref) {
    x += ref.x;
    y += ref.y;
    angle += ref.angle;
    state = (ref.state > state) ? ref.state : state;

    return *this;
  }

  Movement& operator- (Movement& ref) {
    x -= ref.x;
    y -= ref.y;
    angle -= ref.angle;
    state = (ref.state > state) ? ref.state : state;

    return *this;
  }

protected:
  unsigned int startTime = 0;
  unsigned int elapsedTime = 0;
  unsigned int prevTime = 0;
  MovementState state;
  std::shared_ptr<Component> component;

  // Position
  double x;
  double y;
  double angle;

};

} // namespace Movement
} // namespace Graphic
} // namespace story
