#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <memory>

#include <SDL.h>

#include "graphic/layer/Layer.hpp"
#include "graphic/descriptors/ImageDescriptor.hpp"
#include "graphic/descriptors/SpriteDescriptor.hpp"
#include "resource/EImageResource.hpp"
#include "graphic/texture/ESprite.hpp"

class EResourceLoaderInterface;

namespace story {
namespace Resource {

class ResourceBuilder
{
public:
  ResourceBuilder();
  ~ResourceBuilder();

  /* Prohibit copy constructor */
  ResourceBuilder(ResourceBuilder& self) = delete;

/*
 * Scene Functions
 */
  /* Create scene from stored scene descriptor */
  std::shared_ptr<story::Graphic::Layer> createScene(std::string scene_name);

  /* Remove and de-allocate specific scene
   * and related resource(i.e. sprites) from memory */
  void removeScene(std::string scene_name);

/*
 * Image texture Functions
 */
  std::shared_ptr<story::Graphic::EImageTexture>
  createImageTexture(std::shared_ptr<story::Graphic::EImageDesc> imageDesc);

  void removeImageTexture(std::string name);

/*
 * Sprite Functions
 */
  std::shared_ptr<story::Graphic::ESprite>
  createSprite(std::shared_ptr<story::Graphic::ESpriteDesc> spriteDesc
  );

  void removeSprite(std::string name);

/**
 * Low - layer functions
 */
  std::shared_ptr<SDLTextureWrap>
  createTextTexture(std::string text,
      SDL_Color textColor, SDL_Color bgColor,
      std::shared_ptr<SDLFontWrap> font = nullptr);

};

} /* namespace Resource */
} /* namespace story */
