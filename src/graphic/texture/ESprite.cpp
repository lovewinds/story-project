#include "core/Ecore.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResource.hpp"
#include "util/LogHelper.hpp"

#include "graphic/texture/ESprite.hpp"

namespace story {
namespace Graphic {

ESprite::ESprite(std::string _name)
: EDrawable()
{
  sprite_index = 0;
  sprite_change = 1;
  prevTime = 0;
  //controllable = false;

  name = _name;
}

ESprite::~ESprite()
{
  _removeTexture();
  sprite_clips.clear();
}

bool ESprite::initialize(std::shared_ptr<Graphic::ESpriteDesc> spriteDesc)
{
  /* TODO: This logic should be moved proper location */
  /* No blending */
  //setBlendMode(SDL_BLENDMODE_NONE);
  bool result = true;

  source_image_name = spriteDesc->getSource();
  result = _setCellInfo(spriteDesc->getWidth(), spriteDesc->getHeight());
  if (false == result) {
    LOG_ERR("Failed to initialize sprite cells!");
    return false;
  }

  auto indexes = spriteDesc->getIndexes();
  for (size_t index = 1; index <= indexes.size(); index++) {
    _appendSpriteCell(index);
  }

  LOG_DBG("spriteID : %s", name.c_str());
  LOG_DBG("  baseID : %s", source_image_name.c_str());

  //resource = std::shared_ptr<EImageResourceInfo>(new EImageResourceInfo(name, path));
  //LOG_INFO("resource ref. count: [%lu]", resource.use_count());
  ///mTexture = _texture;
  _createTexture();

  if (sprite_clips.size() > 0) {
    mWidth = sprite_clips[0].w;
    mHeight = sprite_clips[0].h;
  }

  return true;
}

bool ESprite::_setCellInfo(unsigned int cell_width, unsigned int cell_height)
{
  Resource::EResourceManager &resManager =
    story::Core::Ecore::getInstance()->getResourceManager();
  std::shared_ptr<Resource::EImageResource> res =
    resManager.getImageResource(source_image_name);

  if (res) {
    this->base_width = res->getWidth();
    this->base_height = res->getHeight();
  } else {
    LOG_ERR("Cannot find source image resource [%s]", source_image_name.c_str());
    return false;
  }

  this->cell_width = cell_width;
  this->cell_height = cell_height;

  LOG_DBG("   SpriteType Name: [%s] / Src: [%s] / Cell: (%d x %d) / Base: (%d x %d)",
      name.c_str(), source_image_name.c_str(),
      cell_width, cell_height,
      base_width, base_height);
  return true;
}

/* Store sprite cell information to create */
bool ESprite::_appendSpriteCell(unsigned int index)
{
  // wid 4
  // cel 1
  // 0 1 2 3
  // 4 5 6 7
  /* Starts from 0 */
  index -= 1;
  SDL_Rect rect;
  unsigned int row_count = (unsigned int)(base_width / cell_width);
  unsigned int col_count = (unsigned int)(base_height / cell_height);
  unsigned int x = (unsigned int)(index % row_count);
  unsigned int y = (unsigned int)(index / row_count);

  if (row_count == 0 || col_count == 0) {
    return false;
  }
  rect.x = cell_width * x;
  rect.y = cell_height * y;
  rect.w = cell_width;
  rect.h = cell_height;
  LOG_DBG("Cell [%d] (%d,%d) [%d/%d]", index, rect.x, rect.y, rect.w, rect.h);

  sprite_clips.push_back(rect);

  return true;
}

std::string ESprite::getName()
{
  return name;
}

void ESprite::setIndex(unsigned int index)
{
  if (index < sprite_clips.size()) {
    sprite_index = index;
    LOG_DBG("Sprite[%p] index set [%d]", this, sprite_index);
  }
}

void ESprite::_createTexture()
{
  story::Resource::EResourceManager& resManager =
    story::Core::Ecore::getInstance()->getResourceManager();
  if (source_image_name.empty()) {
    LOG_ERR("source_image_name is empty !");
    return;
  }

  if (nullptr != mTexture) {
    LOG_ERR("Already allocated.");
    return;
  }

  /* Get texture from ResourceManager */
  sprite_base = resManager.getImageResource(source_image_name);

  if (!sprite_base)
    return;

  mTexture = sprite_base->getTexture();
}

void ESprite::_removeTexture()
{
  mTexture.reset();
  sprite_base->releaseTexture();
}

void ESprite::update(Uint32 currentTime, Uint32 accumulator)
{
  int tick = 100;

  if (sprite_clips.size() == 4)
    tick = 1000;

  if (currentTime - prevTime > tick)
  {
    prevTime = currentTime;

    sprite_index++;
    if (sprite_index >= (int8_t)sprite_clips.size())
      sprite_index = 0;
  }
}

void ESprite::render(
    int base_x, int base_y,
    int delta_x, int delta_y, double delta_angle)
{
  double ani_x = 0, ani_y = 0;

  ani_x = delta_x;
  ani_y = delta_y;

  //texture_render(200, 200, &sprite_clips[sprite_index]);
  //texture_render_resize(100, 100, &sprite_clips[sprite_index], 4);
  if (mTexture) {
    texture_render(
      (int)(base_x + p_x + ani_x),
      (int)(base_y + p_y + ani_y),
      &sprite_clips[sprite_index],
      delta_angle
    );
  }
}

} /* namespace Graphic */
} /* namespace story */
