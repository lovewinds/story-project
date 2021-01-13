#pragma once

#include <functional>
#include "graphic/movement/Component.hpp"

namespace story {
namespace Graphic {
namespace Movement {

class Smooth : public Component {
public:
  Smooth() {
    preDelayTime = 100;
    postDelayTime = 100;
    motionTime = 1000;

    // v = at
    // s = vt == att
    func = [](uint32_t current, uint32_t end) -> Position {
      Position p;
      double constV = 0.4;
      uint32_t constT = 750; // constant time
      uint32_t afterT = (current < constT) ? 0 : (current - constT);
      uint32_t initT = (current < constT) ? current : constT;
      double dA = constV / (end - constT);
      uint32_t dT = end - constT;
      double deltaA = (current < constT) ? 0 : dA;

      p.x = (constV * initT) + (deltaA * dT * afterT / 2.0);
      p.y = 30;
      p.angle = 0.0;
      return p;
    };
  }
  virtual ~Smooth() {};

  virtual Position update(uint32_t currentTime, uint32_t accumulator = 0)
  {
    uint32_t compensated = currentTime + accumulator;
    uint32_t unit = preDelayTime + motionTime + postDelayTime;
    uint32_t actual = compensated % unit;
    // pre delay time
    if (preDelayTime > actual) {
      return Position(0, 0, 0.0);
    }

    // After motion
    uint32_t workTime = actual - preDelayTime;
    if (workTime > motionTime) {
      workTime = motionTime;
    }

    // Compute actual motion
    return func(workTime, motionTime);
  }

};

} // namespace Movement
} // namespace Graphic
} // namespace story
