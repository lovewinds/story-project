#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResource.hpp"

#include "graphic/texture/EGridMapTexture.hpp"

#define T1  16
#define T2  327

namespace story {
namespace Graphic {

EGridMapTexture::EGridMapTexture(std::string name, std::string base_image,
  std::shared_ptr<story::Graphic::EGridDesc> desc) : EDrawable()
{
  this->name = name;
  this->base_image = base_image;

  wTileCount = desc->getGridWidth();
  hTileCount = desc->getGridHeight();
  levels = desc->getGridLevels();

  /* TODO: Load map info dynamically */
  std::vector< std::vector< std::vector<unsigned short> > > arr;
  arr.reserve(hTileCount);
  for (int h = 0; h < hTileCount; h++) {
    std::vector< std::vector<unsigned short> > row;
    row.reserve(wTileCount);
    for (int w = 0; w < wTileCount; w++) {
      std::vector<unsigned short> col;
      col.reserve(levels);
      for (int l = 0; l < levels; l++) {
        unsigned short v = desc->getGridValue(l, w, h);
        col.push_back(v);
      }
      row.push_back(col);
    }
    arr.push_back(row);
  }

  tileMap = arr;
  _createTexture();
}

EGridMapTexture::~EGridMapTexture()
{
  /* Deallocate */
  _removeTexture();
}

void EGridMapTexture::_createTexture()
{
  story::Resource::EResourceManager& resManager =
    story::Core::Ecore::getInstance()->getResourceManager();
  if (base_image.empty()) {
    LOG_ERR("base_image is empty !");
    return;
  }

  tile_image = resManager.getImageResource(base_image);
  if (!tile_image) {
    LOG_ERR("ImageResource is not found !");
    return;
  }

  if (mWidth == 0)
    mWidth = tile_image->getWidth();
  if (mHeight == 0)
    mHeight = tile_image->getHeight();

  /* Get shared texture from Resource Manager */
  mTexture = tile_image->getTexture();
}

void EGridMapTexture::_removeTexture()
{
  mTexture.reset();
  if (nullptr != tile_image)
    tile_image->releaseTexture();
}

void EGridMapTexture::update(Uint32 currentTime, Uint32 accumulator)
{
}

void EGridMapTexture::render(
    int base_x, int base_y,
    int delta_x, int delta_y, double delta_angle)
{
  SDL_Rect rect = { 0, };
  const unsigned int base_width = 240;
  const unsigned int base_height = 280;
  const unsigned int cell_width = 40;
  const unsigned int cell_height = 40;
  unsigned int row_count = (unsigned int)(base_width / cell_width);
  unsigned int col_count = (unsigned int)(base_height / cell_height);
  unsigned int tile = 0;

  if (row_count == 0 || col_count == 0) {
    LOG_ERR("Invalid map information");
  return;
  }

  /* TODO: IMPORTANT !
   * This dumb logic should be improved.
   * Current it renders every single tile seperatedly, it may affect rendering speed.
   * AND also makes outline of tile issue also!
   * -> Patched temporary at Line #397 at Ecore.cpp
   *    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")
   *                                                ^
   * It disables linear scaling :(
   */
  if (mTexture) {
    for (int y = 0; y < hTileCount; y++) {
      for (int x = 0; x < wTileCount; x++) {
        for (int l = 1; l < levels; l++) {
          tile = (unsigned int)tileMap[y][x][l];
          if (tile == 9999) continue;
          unsigned int tx = (unsigned int)(tile % row_count);
          unsigned int ty = (unsigned int)(tile / row_count);

          rect.x = cell_width * tx;
          rect.y = cell_height * ty;
          rect.w = cell_width;
          rect.h = cell_height;

          texture_render((int)(x*cell_width), (int)(y*cell_height), &rect, 0.0, true);
        }
      }
    }
  }
}

int EGridMapTexture::getWidth()
{
  return mWidth;
}

int EGridMapTexture::getHeight()
{
  return mHeight;
}

void EGridMapTexture::setPos(int x, int y)
{
  p_x = x;
  p_y = y;
}

std::string EGridMapTexture::getName()
{
  return name;
}

} /* namespace Graphic */
} /* namespace story */
