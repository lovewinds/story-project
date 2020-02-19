#pragma once

#include <string>
#include <cmath>

#include "graphic/texture/Texture.hpp"

namespace story {
namespace Resource {
  class EImageResource;
}
}

namespace story {
namespace Graphic {
class ImageTexture : public Texture
{
public:
  /* Initializes variables */
  ImageTexture(std::string name, std::string base_image);

  /* Deallocates memory */
  ~ImageTexture();

  virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
  //virtual void paint(Uint32 currentTime, Uint32 accumulator = 0);
  virtual void render(
    int base_x, int base_y,
    int delta_x, int delta_y, double delta_angle);

  /* Gets image dimensions */
  int getWidth();
  int getHeight();
  void setWidth(double width, bool ratio = false);
  void setHeight(double height, bool ratio = false);
  std::string getName();

protected:
  void _createTexture();
  void _removeTexture();

  int radian;
  double wRatio;
  double hRatio;

  /* Angle of rotation */
  double m_degrees;

  /* animation */
  bool animating;

  std::string name;
  std::string base_image;

  std::shared_ptr<story::Resource::EImageResource>  image;
};

} /* namespace Graphic */
} /* namespace story */
