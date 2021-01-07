#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"
#include "graphic/movement/Movement.hpp"

namespace story {
namespace Graphic {

Movement::Movement()
    : x(0.0), y(0.0), angle(0.0)
{
  state = MOVEMENT_STOP;
}

Movement::Movement(double x, double y, double angle)
    : x(x), y(y), angle(angle)
{
  state = MOVEMENT_STOP;
}

void Movement::start()
{
  prevTime = startTime = story::Core::Ecore::getAppTicks();
  state = MOVEMENT_START;
}

void Movement::stop()
{
  startTime = 0;
  state = MOVEMENT_STOP;
}

void Movement::sync()
{
}


void Movement::pause()
{
  elapsedTime = story::Core::Ecore::getAppTicks() - startTime;
  state = MOVEMENT_PAUSE;
}

void Movement::resume()
{
  startTime = story::Core::Ecore::getAppTicks() - elapsedTime;
  state = MOVEMENT_START;
}

void Movement::update(unsigned int currentTime, unsigned int accumulator)
{
  static double accel = 0.026;
  static double velo = 1.0;

  Uint32 compensatedTime = currentTime;
  Uint32 atomicTime = (compensatedTime - startTime);
  Uint32 delta = atomicTime - prevTime;

  if (atomicTime == 0) {
    velo = 1.0;
    accel = 0.0;
  }

  if (MOVEMENT_START != state) return;

  accel = 0.0;
  velo = 0.1;

  //prev_y = a_y + velo * ((atomicTime > 375) ? 375 : atomicTime);
  y = velo * ((atomicTime > 375) ? 375 : atomicTime);

  if (atomicTime > 375) {
    delta = atomicTime - 375;
    accel = -0.00026;
    //velo = 1.0 - (1.0 / 375.0) * (delta - 375);
    velo += accel * delta;
    y += velo * delta;
  }

  /* Recover */
  if (y < -0.1) {
    y = 0.0;
    startTime = currentTime;
    accel = 0.0;
    velo = 0.1;
  }

  x = y;
  prevTime = delta;
}


} /* namespace Graphic */
} /* namespace story */
