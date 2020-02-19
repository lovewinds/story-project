#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "graphic/texture/ImageTexture.hpp"
#include "graphic/texture/TextTexture.hpp"
#include "graphic/texture/FigureTexture.hpp"
#include "graphic/GraphicObject.hpp"
#include "graphic/animation/EAnimation.hpp"

namespace story {
namespace Graphic {

/**
 * Layer
 *
 * This class indicates specific graphic layer on screen.
 *  - It should own textures to show.
 *  - It should perform render and update.
 */
class Layer
{
public:
  Layer(std::string name);
  virtual ~Layer();

  /* Primitive Scene actions */
  virtual void handleEvent(SDL_Event e);
  virtual void render();
  virtual void update(Uint32 currentTime, Uint32 accumulator);

  std::string getName();

  /* Should handle creation of resources with ResourceBuilder */
  virtual bool addObject(std::shared_ptr<Graphic::Object> object);

  /* Resource manager allocates image resource */
  //std::shared_ptr<SDLTextureWrap> allocateTexture(std::string path);
  //void releaseTexture(std::string path);

  void setActiveState(bool active);

  /* Export read-only resources */
  const std::map<std::string, std::shared_ptr<SpriteTexture>>& sprite_map;

protected:
  Layer();
  std::string name;

  /* TODO: Scene is activated, so it perform update and render */
  bool isActivated;

  /**
   * Texture map
   *   - All these pointers of texture resources are stored here
   *     to repeated search logic in ResourceManager.
   *   - Also, shared_ptr need to be stored here also
   *     for increase ref count.
   */
  std::map<std::string, std::shared_ptr<SpriteTexture>> _sprite_map;

  std::map<std::string, std::shared_ptr<ImageTexture>> _img_texture_map;

  std::map<std::string, std::shared_ptr<TextTexture>> _text_texture_map;

  std::map<std::string, std::shared_ptr<FigureTexture>> _drawable_map;

  /* Not classified textures */
  std::map<std::string, std::shared_ptr<Texture>> _raw_texture_map;

  std::map<std::string, std::shared_ptr<Graphic::Object>> _object_map;
};

} /* namespace Graphic */
} /* namespace story */
