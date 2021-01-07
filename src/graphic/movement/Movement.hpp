#pragma once

#include <iostream>
#include <memory>

namespace story {
namespace Graphic {

enum MovementState
{
  MOVEMENT_NONE,
  MOVEMENT_STOP,
  MOVEMENT_START,
  MOVEMENT_PAUSE,
  MOVEMENT_RESUME,
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

  /* Position */
  double x;
  double y;
  double angle;
};

} /* namespace Graphic */
} /* namespace story */
