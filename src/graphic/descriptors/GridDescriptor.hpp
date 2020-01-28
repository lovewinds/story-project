#pragma once

#include <string>

namespace story {
namespace Graphic {

/*
 * Grid Descriptor class
 * This class describes layered-grid map structure.
 */

enum EGridLayerID {
	GRID_LAYER_UNMOVABLE = 0,
	GRID_LAYER_MOVABLE,
	GRID_LAYER_EVENT,
	GRID_LAYER_END /* The number of layers. Do not change this */
};

class EGridDesc
{
public:
    EGridDesc(int width, int height, int levels) {
        this->width = width;
        this->height = height;
        this->levels = levels;

        gridArray = new unsigned short **[width];
        for(int col = 0; col < width; col++) {
            gridArray[col] = new unsigned short *[height];
            for(int row = 0; row < height; row++) {
                gridArray[col][row] = new unsigned short[levels];
                for (int ll = 0; ll < levels; ll++) {
                gridArray[col][row][ll] = 9999;
                }
            }
        }
    }
    virtual ~EGridDesc() {
        for(int col = 0; col < width; col++) {
            for(int row = 0; row < height; row++) {
                delete[] gridArray[col][row];
            }
            delete[] gridArray[col];
        }
        delete[] gridArray;
    }

    void setGridValue(int level, int x, int y, unsigned short value) {
        if (0 <= x && x < width) {
            if (0 <= y && y < height) {
                if (0 <= level && level < levels) {
                    gridArray[x][y][level] = value;
                }
            }
        }
    }
    unsigned short getGridValue(int level, int x, int y) {
        unsigned short res = 9999;
        if (0 <= x && x < width) {
            if (0 <= y && y < height) {
                if (0 <= level && level < levels) {
                    res = gridArray[x][y][level];
                }
            }
        }
        return res;
    }
    int getGridWidth() { return width; }
    int getGridHeight() { return height; }
    int getGridLevels() { return levels; }
    void setBaseImage(std::string base) {
        base_image = base;
    }
    std::string getBaseImage() { return base_image; }

protected:
    int width;
    int height;
    int levels;
    std::string base_image;
    unsigned short ***gridArray;
};

} /* namespace Graphic */
} /* namespace story */
