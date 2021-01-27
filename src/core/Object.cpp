#pragma once

#include "core/Object.hpp"
#include "util/LogHelper.hpp"
#include "graphic/movement/Component.hpp"

namespace story {
namespace Core {

void Object::render() {
    Graphic::Movement::Position p(0.0, 0.0, 0.0);
    if (movement) {
        p.x += movement->getX();
        p.y += movement->getY();
    }
    graphicObject->render(p.x, p.y);
}

void Object::update(Uint32 currentTime, Uint32 accumulator) {
    graphicObject->update(currentTime, accumulator);
    if (movement) movement->update(currentTime, accumulator);
}

} // namespace Core
} // namespace story
