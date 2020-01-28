#pragma once

#include <string>

namespace story {
namespace Graphic {

class ESpriteDesc
{
public:
    ESpriteDesc(std::string name, std::string sourceType, int x, int y) {
        this->name = name;
        this->type = sourceType;
        this->pos_x = x;
        this->pos_y = y;
        controllable = false;
    }

    void setControllable(bool controllable) {
        this->controllable = controllable;
    }
    bool isControllable() { return controllable; }

    std::string getName() { return name; }
    std::string getType() { return type; }
    int getX() { return pos_x; }
    int getY() { return pos_y; }

protected:
    std::string name;
    std::string type;
    int pos_x;
    int pos_y;
    bool controllable;
};

} /* namespace Graphic */
} /* namespace story */
