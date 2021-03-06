﻿#if !defined(PLATFORM_WINDOWS)
#pragma execution_character_set("utf-8")
#endif

#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "graphic/texture/FigureTexture.hpp"
#include "resource/ResourceManager.hpp"
#include "graphic/GraphicObject.hpp"
#include "graphic/animation/EAccelAnimation.hpp"

#include "screen/layer/ChatLayer.hpp"

namespace story {
namespace Screen {

ChatLayer::ChatLayer(std::string name)
 : ScreenLayer(name)
{
  LOG_INFO("ChatLayer[%s] created", name.c_str());

  //SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
  //SDL_Color bgColor = { 0x0, 0x0, 0x0 };
  //SDL_Color boxBGColor = { 0x70, 0xC6, 0xFF };
  SDL_Color textColor = { 0x00, 0x00, 0x00 };
  SDL_Color bgColor = { 0xFF, 0xFF, 0xFF };
  SDL_Color boxBGColor = { 0xE3, 0xE3, 0xB4 };
  std::shared_ptr<Graphic::Object> object(new Graphic::Object());

  std::shared_ptr<Graphic::FigureTexture> dr(new Graphic::FigureTexture(
    10, story::Core::Ecore::getScreenHeight() / 3 * 2,
    story::Core::Ecore::getScreenWidth() - 20, (story::Core::Ecore::getScreenHeight() / 3) - 10,
    boxBGColor));
  auto result = _drawable_map.emplace("Talkbox", dr);
  if (!result.second) {
    LOG_ERR("Failed to insert Drawable !");
  }

  /* TODO: UTF-8 char should not be used directly. */
  std::shared_ptr<Graphic::TextTexture> txt(
    new Graphic::TextTexture("안녕?", textColor, bgColor));
  object->setName("Message");
  object->movePositionTo(20, (story::Core::Ecore::getScreenHeight() / 3 * 2) + 10);
  object->addText(txt);

  addObject(object);
}

ChatLayer::~ChatLayer()
{
  LOG_INFO("ChatLayer[%s] removed", name.c_str());
}

void ChatLayer::testAnimation(Graphic::AnimationState state)
{
  std::shared_ptr<Graphic::EAnimation> ani;
  //for (auto& it : _img_texture_map)
  for (auto& it : _object_map)
  {
    auto& object = it.second;
    switch (state) {
    case Graphic::ANI_STOP:
      object->stopAnimation();
      break;
    case Graphic::ANI_START:
      ani = std::shared_ptr<Graphic::EAnimation>(new Graphic::EAccelAnimation());
      object->setAnimation(ani);
      object->startAnimation();
      /* Update position animation finished? */
      break;
    case Graphic::ANI_PAUSE:
      object->pauseAnimation();
      break;
    case Graphic::ANI_RESUME:
      object->resumeAnimation();
      break;
    default:
      break;
    }
  }
#if 0
  for (auto& it : _drawable_map)
  {
    auto& drawable = it.second;
    switch (state) {
    case ANI_STOP:
      drawable->stopAnimation();
      break;
    case ANI_START:
      ani = std::shared_ptr<EAnimation>(new EAnimation());
      drawable->setAnimation(ani);
      drawable->startAnimation();
      /* Update position animation finished? */
      break;
    case ANI_PAUSE:
      drawable->pauseAnimation();
      break;
    case ANI_RESUME:
      drawable->resumeAnimation();
      break;
    }
  }
#endif
}

void ChatLayer::handleEvent(SDL_Event e)
{
  /* Handler events for Scene instance */
  if (e.type == SDL_KEYDOWN) {
    LOG_INFO("Pressed [%d]", e.key.keysym.sym);
    switch (e.key.keysym.sym) {
    case SDLK_9:
      LOG_INFO("Play Animation");
      testAnimation(Graphic::ANI_START);
      break;
    case SDLK_0:
      LOG_INFO("Stop Animation");
      testAnimation(Graphic::ANI_STOP);
      break;
    case SDLK_MINUS:
      LOG_INFO("Pause Animation");
      testAnimation(Graphic::ANI_PAUSE);
      break;
    case SDLK_EQUALS:
      LOG_INFO("Resume Animation");
      testAnimation(Graphic::ANI_RESUME);
      break;
    }
  }
}

void ChatLayer::render()
{
  for (auto &it : _img_texture_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _sprite_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _drawable_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _text_texture_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _object_map)
  {
    it.second->render();
  }
}

void ChatLayer::update(Uint32 currentTime, Uint32 accumulator)
{
  if (false == isActivated)
    return;

  for (auto& it : _sprite_map)
  {
    it.second->update(currentTime, accumulator);
  }

  for (auto &it : _img_texture_map)
  {
    it.second->update(currentTime, accumulator);
  }

  for (auto& it : _drawable_map)
  {
    it.second->update(currentTime, accumulator);
  }

  for (auto& it : _text_texture_map)
  {
    it.second->update(currentTime, accumulator);
  }

  for (auto& it : _object_map)
  {
    it.second->update(currentTime, accumulator);
  }

}

} /* namespace Graphic */
} /* namespace story */
