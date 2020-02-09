#pragma once

#include <string>

namespace story {
namespace Graphic {

class ESpriteDesc
{
public:
  ESpriteDesc(std::string name, std::string source) {
    this->name = name;
    this->source_image = source;
    this->object_name = "";
    controllable = false;
  }

  void setControllable(bool controllable) {
    this->controllable = controllable;
  }
  bool isControllable() { return controllable; }
  void appendSpriteCellIndex(unsigned int index) {
    index_array.push_back(index);
  }

  void setObjectName(std::string object_name) { this->object_name = object_name; }
  std::string getName() { return name; }
  std::string getSource() { return source_image; }
  std::string getObjectName() { return object_name; }
  void setX(int x) { pos_x = x; }
  void setY(int y) { pos_y = y; }
  int getX() { return pos_x; }
  int getY() { return pos_y; }
  std::vector<unsigned int> getIndexes() { return index_array; }

  void setWidth(int width) { cell_width = width; }
  void setHeight(int height) { cell_height = height; }
  int getWidth() { return cell_width; }
  int getHeight() { return cell_height; }

protected:
  std::string name;
  std::string object_name;
  std::string source_image;
  int pos_x;
  int pos_y;
  bool controllable;

  std::vector<unsigned int> index_array;

  /* Width and Height of each cell */
  unsigned int cell_width;
  unsigned int cell_height;
};

} /* namespace Graphic */
} /* namespace story */
