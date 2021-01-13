#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"
#include "graphic/movement/Movement.hpp"

namespace story {
namespace Graphic {
namespace Movement {

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
  if (MOVEMENT_START == state && component) {
    Position p = component->update(currentTime, accumulator);
    x = p.x;
    y = p.y;
    angle = p.angle;
  }
}

} // namespace Movement
} // namespace Graphic
} // namespace story
