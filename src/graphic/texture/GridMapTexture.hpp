#pragma once

#include <string>
#include <cmath>
#include <vector>

#include "graphic/texture/Texture.hpp"

namespace story {
namespace Resource {
  class EImageResource;
}
}

namespace story {
namespace Graphic {

class GridMapTexture : public Texture
{
public:
  /* Initializes variables */
  GridMapTexture(std::string name, std::string base_image,
      std::shared_ptr<story::Graphic::EGridDesc> desc);
  ~GridMapTexture();

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
  virtual void render(
    int base_x, int base_y,
    int delta_x, int delta_y, double delta_angle);

  /* Gets image dimensions */
  int getWidth();
  int getHeight();
  void setPos(int x, int y);
  void setTileSize(unsigned int width, unsigned int height);
  std::string getName();

protected:
  void _createTexture();
  void _removeTexture();

  unsigned int wTileCount;
  unsigned int hTileCount;
  unsigned int tile_width;
  unsigned int tile_height;
  unsigned int levels;

  std::string name;
  std::string base_image;

  std::vector< std::vector< std::vector<unsigned short> > > tileMap;

  std::shared_ptr<story::Resource::EImageResource>  tile_image;
};

} /* namespace Graphic */
} /* namespace story */
