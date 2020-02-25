#pragma once

#include <string>

namespace story {
namespace Scenario {

class SceneBuilder
{
public:
  SceneBuilder(std::string name) {
    this->name = name;
  }

  SceneBuilder& addLayer() {
    return *this;
  }


protected:
  std::string name;
};

} /* namespace Scenario */
} /* namespace story */
