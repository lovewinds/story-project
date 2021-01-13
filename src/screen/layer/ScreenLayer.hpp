#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "core/Object.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "graphic/texture/ImageTexture.hpp"
#include "graphic/texture/TextTexture.hpp"
#include "graphic/texture/FigureTexture.hpp"
#include "graphic/GraphicObject.hpp"
#include "graphic/animation/EAnimation.hpp"

namespace story {
namespace Screen {

class ScreenLayer
{
public:
  ScreenLayer(std::string name);
  virtual ~ScreenLayer();

  /* Primitive Scene actions */
  virtual void handleEvent(SDL_Event e);
  virtual void render();
  virtual void update(Uint32 currentTime, Uint32 accumulator);

  std::string getName();

  /* Should handle creation of resources with ResourceManager */
  virtual bool addObject(std::shared_ptr<Graphic::Object> object);
  virtual bool addCoreObject(std::shared_ptr<Core::Object> object);

  /* Resource manager allocates image resource */
  //std::shared_ptr<SDLTextureWrap> allocateTexture(std::string path);
  //void releaseTexture(std::string path);

  void setActiveState(bool active);

  /* Export read-only resources */
  const std::map<std::string, std::shared_ptr<Graphic::SpriteTexture>>& sprite_map;

protected:
  ScreenLayer();
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
  std::map<std::string, std::shared_ptr<Graphic::SpriteTexture>> _sprite_map;

  std::map<std::string, std::shared_ptr<Graphic::ImageTexture>> _img_texture_map;

  std::map<std::string, std::shared_ptr<Graphic::TextTexture>> _text_texture_map;

  std::map<std::string, std::shared_ptr<Graphic::FigureTexture>> _drawable_map;

  /* Not classified textures */
  std::map<std::string, std::shared_ptr<Graphic::Texture>> _raw_texture_map;

  std::map<std::string, std::shared_ptr<Graphic::Object>> _object_map;

  std::map<std::string, std::shared_ptr<Core::Object>> _core_object_map;
};

} /* namespace Screen */
} /* namespace story */
