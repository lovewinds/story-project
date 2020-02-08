#pragma once

#include <iostream>
#include <memory>

namespace story {
  namespace Screen {
    /* Avoid circular reference */
    class Object;
  }
}

namespace story {
namespace Graphic {

enum AnimationRepeatType
{
  ANI_NO_REPEAT,
  ANI_REPEAT
};

enum AnimationState
{
  ANI_NONE,
  ANI_STOP,
  ANI_START,
  ANI_PAUSE,
  ANI_RESUME,
};

enum RotateDirection {
  ROTATE_CLOCKWISE,
  ROTATE_ANTICLOCKWISE
};

class EAnimation
{
public:
  EAnimation();
  virtual ~EAnimation();

  virtual void start();
  virtual void stop();
  virtual void pause();
  virtual void resume();
  virtual void sync();

  double getX();
  double getY();
  double getAngle();
  AnimationState getState();

  void setCaller(std::weak_ptr<story::Screen::Object> caller);

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
  Uint32 startTime = 0;
  Uint32 elapsedTime = 0;
  Uint32 prevTime = 0;
  AnimationState state;

  /* This model only supports one callback at one time. */
  /* TODO: Use callback list */
  //std::weak_ptr<story::Screen::Object> caller;

  std::weak_ptr<story::Screen::Object> caller;

  /* Position */
  double a_x;
  double a_y;
  double a_angle;
};

} /* namespace Graphic */
} /* namespace story */
