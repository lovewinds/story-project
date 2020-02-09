#pragma once

#include <string>
#include <memory>
#include <vector>

#include "graphic/texture/EDrawable.hpp"
#include "graphic/descriptors/SpriteDescriptor.hpp"

namespace story {
namespace Resource {
  class EImageResource;
}
}

namespace story {
namespace Graphic {

class ESprite : public EDrawable
{
public:
  ESprite(std::string name);
  ~ESprite();

  bool initialize(std::shared_ptr<Graphic::ESpriteDesc> spriteDesc);

  std::string getName();
  void setIndex(unsigned int index);
  //void setControllable(bool controllable);
  //bool isControllable();

  /* Store sprite cell information to create */
  bool appendSpriteCell(unsigned int index);

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
  virtual void render(
    int base_x, int base_y,
    int delta_x, int delta_y, double delta_angle);

protected:
  ESprite();
  bool _setCellInfo(unsigned int cell_width, unsigned int cell_height);
  bool _appendSpriteCell(unsigned int index);

  void _createTexture();
  void _removeTexture();

  std::string name;
  std::string source_image_name;

  /* Width and Height of each cell */
  unsigned int cell_width;
  unsigned int cell_height;

  /* Width and Height of base image */
  unsigned int base_width;
  unsigned int base_height;
  std::vector<SDL_Rect> sprite_clips;
  int8_t  sprite_index;
  int8_t  sprite_change;

  //bool controllable;

  Uint32 prevTime;

  std::shared_ptr<story::Resource::EImageResource> sprite_base;
};

} /* namespace Graphic */
} /* namespace story */
