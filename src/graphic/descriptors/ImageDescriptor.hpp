#pragma once

#include <string>

namespace story {
namespace Graphic {

/* TODO: Should be expanded to all objects */
enum EImageHorizontalAlign {
	IMAGE_ALIGN_LEFT = 0,
	IMAGE_ALIGN_RIGHT
};

enum EImageVerticalAlign {
	IMAGE_ALIGN_TOP = 0,
	IMAGE_ALIGN_BOTTOM
};

class EImageDesc
{
public:
    EImageDesc(std::string name, std::string sourceType, int x, int y) {
        this->name = name;
        this->type = sourceType;
        this->pos_x = x;
        this->pos_y = y;
        this->h_align = IMAGE_ALIGN_LEFT;
        this->v_align = IMAGE_ALIGN_TOP;
    }

    void setWidth(double width) {
        this->width = width;
    }
    void setHeight(double height) {
        this->height = height;
    }
    void setWidthRatio(double width) {
        this->width_ratio = width;
    }
    void setHeightRatio(double height) {
        this->height_ratio = height;
    }
    void setHorizontalAlign(EImageHorizontalAlign h_align) {
        this->h_align = h_align;
    }
    void setVerticalAlign(EImageVerticalAlign v_align) {
        this->v_align = v_align;
    }

    std::string getName() { return name; }
    std::string getType() { return type; }
    int getX() { return pos_x; }
    int getY() { return pos_y; }
    double getWidth() { return width; }
    double getHeight() { return height; }
    double getWidthRatio() { return width_ratio; }
    double getHeightRatio() { return height_ratio; }
    EImageHorizontalAlign getHorizontalAlign() { return h_align; }
    EImageVerticalAlign getVerticalAlign() { return v_align; }

protected:
    std::string name;
    std::string type;
    int pos_x;
    int pos_y;
    double width;
    double height;
    double width_ratio;
    double height_ratio;

    EImageHorizontalAlign h_align;
    EImageVerticalAlign v_align;
};

} /* namespace Graphic */
} /* namespace story */
