#if !defined(PLATFORM_WINDOWS)
#pragma execution_character_set("utf-8")
#endif

#include "core/Ecore.hpp"
#include "core/ScreenManager.hpp"
#include "util/LogHelper.hpp"
#include "graphic/texture/ESprite.hpp"
#include "graphic/texture/EFigure.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/ResourceBuilder.hpp"
#include "graphic/GraphicObject.hpp"
#include "graphic/animation/EAccelAnimation.hpp"

#include "graphic/layer/TitleLayer.hpp"

namespace story {
namespace Graphic {

TitleLayer::TitleLayer(std::string name)
 : Layer(name)
{
  LOG_INFO("TitleLayer[%s] created", name.c_str());

  initMenuItem();
}

TitleLayer::~TitleLayer()
{
  LOG_INFO("TitleLayer[%s] removed", name.c_str());
}

void TitleLayer::initMenuItem()
{
  SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
  SDL_Color bgColor = { 0x00, 0x00, 0x00 };
  SDL_Color listBGColor = { 0x30, 0x30, 0x30, 0xA0 };
  char str_id[32] = { 0, };
  char str_text[32];

  /* Menu list items */
  for (int i = 0; i < 3; i++) {
    int item_x = 50;
    int item_y = story::Core::Ecore::getScreenHeight() - 180 + (50 * i);
    listBGColor.r = listBGColor.g = listBGColor.b = 0x30 + (0xA * i);

    SDL_snprintf(str_id, 32, "menu_%d", i + 1);

    std::shared_ptr<EFigure> dr(new EFigure(item_x, item_y, 100, 40, listBGColor));
    auto result = _drawable_map.emplace(str_id, dr);
    if (!result.second) {
      LOG_ERR("Failed to insert Drawable !");
    }

    /* TODO: UTF-8 char should not be used directly. */
    SDL_snprintf(str_text, 32, "메뉴 %d", i + 1);
    std::shared_ptr<ETextTexture> txt(
      new ETextTexture(str_text, textColor, bgColor));

    std::shared_ptr<story::Graphic::Object> object(new story::Graphic::Object());
    object->setName(str_id);
    object->movePositionTo(item_x, item_y);
    object->addText(txt);
    addObject(object);
  }

/* Title text */
  std::shared_ptr<ETextTexture> txt(
      new ETextTexture("Story", textColor, bgColor, 58));
  std::shared_ptr<story::Graphic::Object> title_obj(new story::Graphic::Object());

  title_obj->setName("title_string");
  title_obj->movePositionTo(story::Core::Ecore::getScreenWidth()-200, 100);
  title_obj->addText(txt);
  addObject(title_obj);



/* Create dynamic animations */
/* Should be moved into ObjectFactory */
  story::Resource::EResourceManager& resManager =
    story::Core::Ecore::getInstance()->getResourceManager();
  int s_width = story::Core::Ecore::getScreenWidth();
  int s_height = story::Core::Ecore::getScreenHeight();

  for (int i = 0; i < 64; i++) {
    std::shared_ptr<EImageTexture> imgTexture = nullptr;
    std::shared_ptr<story::Graphic::Object> object
          (new story::Graphic::Object());
    int obj_x = rand() % s_width;
    int obj_y = rand() % s_height;
    int dest_y = 0;
    int transition = 0;

    if (obj_y < (s_height / 2))
      dest_y = obj_y + (s_height / 8);
    else
      dest_y = obj_y - (s_height / 8);

    SDL_snprintf(str_text, 32, "icon_%d", i + 1);
    if (i % 4 == 0)
      imgTexture = resManager.createImageTexture(str_text, "icon_circle");
    else if (i % 4 == 1)
      imgTexture = resManager.createImageTexture(str_text, "icon_cross");
    else if (i % 4 == 2)
      imgTexture = resManager.createImageTexture(str_text, "icon_triangle");
    else
      imgTexture = resManager.createImageTexture(str_text, "icon_rectangle");

    if (nullptr == imgTexture) {
      LOG_ERR("Failed to create Image !");
    } else {
      imgTexture->setWidth(20, true);
      imgTexture->setHeight(20, true);
      imgTexture->setAlpha(70);
      object->setName(str_text);
      object->movePositionTo(obj_x, obj_y);
      object->addImage(imgTexture);

      transition = ((rand() % 3) + 1) * 5000;
      object->animatedMoveRotateTo(object, obj_x, dest_y, transition);

      addObject(object);
    }
  }
}

void TitleLayer::sampleMenuState(std::string id)
{
  for (auto& it : _object_map)
  {
    auto& object = it.second;
    // int obj_x = object->getPositionX();
    int obj_y = object->getPositionY();

    if (object->getName().find("menu_") != std::string::npos)
    {
      LOG_DBG(" matched [%s]", object->getName().c_str());
      /* Prefix matched */
      if (object->getName() == id) {
        // Do select
        LOG_DBG("  + Selected [%s]", object->getName().c_str());
        object->animatedMoveTo(object, 80, obj_y, 300);
      } else {
        // DO unselect
        LOG_DBG("  - Unselected [%s]", object->getName().c_str());
        object->animatedMoveTo(object, 50, obj_y, 300);
      }
    }
  }
}

/* TODO: Logic should be changed to register clickable position
  for each object */
bool TitleLayer::checkMenuClicked(int x, int y)
{
  for (auto& it : _object_map)
  {
    std::shared_ptr<story::Graphic::Object> object = it.second;
    if (nullptr == object) continue;

    int obj_x = object->getPositionX();
    int obj_y = object->getPositionY();
    int obj_x2 = object->getPositionX() + object->getWidth();
    int obj_y2 = object->getPositionY() + object->getHeight();

    if (obj_x <= x && x <= obj_x2 &&
      obj_y <= y && y <= obj_y2)
    {
      /* Only propagate for list items */
      if (object->getName().find("menu_") != std::string::npos) {
        if (object->getAnimationState() != ANI_START) {
          sampleMenuState(object->getName());
          return true;
        }
      } else if (object->getName() == "title_string") {
        /* Title */

        /* Make a request to change scene */
        /* WARNING : DO NOT call playScene inside of another scene logic */
        std::string s("sample_rpg");
        LOG_DBG("Request to play scene [%s]", s.c_str());
        story::Core::Ecore::requestSceneChange(s);
      }
    }
  }
  return false;
}

void TitleLayer::handleEvent(SDL_Event e)
{
  /* Handler events for Scene instance */
  if (e.type == SDL_MOUSEBUTTONDOWN) {
    LOG_INFO("Clicked [%03d, %03d]", e.button.x, e.button.y);

    /* TODO: event consumer should be handle more efficient way */
    bool consumed = checkMenuClicked(e.button.x, e.button.y);
    if (consumed)
      return;
  }
}

void TitleLayer::render()
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

void TitleLayer::update(Uint32 currentTime, Uint32 accumulator)
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
