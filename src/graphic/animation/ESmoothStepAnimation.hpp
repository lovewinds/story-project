#pragma once

#include "graphic/animation/EAnimation.hpp"

namespace story {
namespace Graphic {

class ESmoothStepAnimation : public EAnimation
{
public:
  ESmoothStepAnimation();
  virtual ~ESmoothStepAnimation();

  virtual void setStartPosition(int x, int y);
  virtual void setEndPosition(int x, int y);
  virtual void setTransition(int milliseconds);

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
  int start_x = 0;
  int start_y = 0;
  int end_x = 0;
  int end_y = 0;
  int transition_msec = 1000;
};

} /* namespace Graphic */
} /* namespace story */
