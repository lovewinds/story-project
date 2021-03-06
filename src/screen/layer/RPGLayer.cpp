#include <iostream>
#include <sstream>
#include <limits>

#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/ResourceManager.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "graphic/animation/EGridMoveAnimation.hpp"

#include "screen/layer/RPGLayer.hpp"

namespace story {
namespace Screen {

RPGLayer::RPGLayer(std::string name)
 : ScreenLayer(name)
{
  LOG_INFO("RPGLayer[%s] created", name.c_str());
#if 0
  ResourceManager& resManager = story::Core::Ecore::getInstance()->getResourceManager();
  SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
  SDL_Color bgColor = { 0x0, 0x0, 0x0 };
  std::shared_ptr<TextTexture> tt(new TextTexture("test", textColor, bgColor));
  tt->movePositionTo(10, 100);
  auto result = _text_texture_map.emplace("test string", tt);
  if (!result.second) {
    LOG_ERR("Failed to insert text set !");
  }
#endif

#if 0
  /* Create base map textures */
  ResourceManager& resManager = story::Core::Ecore::getInstance()->getResourceManager();
  /* Create sprite by manually for test */

  std::shared_ptr<Graphic::Object> object(new Graphic::Object());
  std::shared_ptr<SpriteTexture> sprite;

  sprite = resManager.createSprite("char_girl", "movingChar");
  object->setName(sprite->getName());
  object->movePositionTo(10, 10);
  object->addSprite(sprite);
  
  auto ins = _object_map.emplace("movingChar", object);
  if (!ins.second) {
    LOG_ERR("Failed to insert object map!");
  }
#endif
}

RPGLayer::~RPGLayer()
{
  gridMap.reset();

  LOG_INFO("RPGLayer[%s] removed", name.c_str());
}

/* Store Image and Sprite to render position based */
bool RPGLayer::addObject(std::shared_ptr<Graphic::Object> object)
{
  double _x = 0.0;
  double _y = 0.0;
  std::ostringstream stringStream;
  std::string position_str;
  unsigned long pos = 0;

  LOG_DBG("Check object position [%3d, %3d]",
    (int)object->getPositionX(), (int)object->getPositionY());

  _x = object->getPositionX() * 32.0;
  _y = object->getPositionY() * 32.0;
  object->movePositionTo(_x, _y);

  /* Make position based string
   *    YYYYXXXX
   */
  pos = ((unsigned int)_y & 0xFFFF) << 16;
  pos = pos + ((unsigned int)_x & 0xFFFF);
  stringStream << std::hex << std::setfill('0') << std::setw(8)
    << pos << std::dec
    << "_" << object->getName();
  position_str = stringStream.str();
  LOG_DBG("POS [%s]", position_str.c_str());

  if (object->isControllable()) {
    object->setPositionCallback(
      std::bind(&RPGLayer::objectPositionCallback, this,
        std::placeholders::_1, std::placeholders::_2));
  }

  /* Convert into actual position from grid position */
  auto result = _object_map.emplace(position_str, object);
  if (!result.second) {
    LOG_ERR("Failed to insert object map!");
    return false;
  }
  return true;
}

void RPGLayer::setMap(std::shared_ptr<Graphic::GridMapTexture> map)
{
  //std::shared_ptr<GridMapTexture> map(new GridMapTexture("MyMap", "MapTile"));
  auto result = _raw_texture_map.emplace("GridMap", map);
  if (!result.second) {
    LOG_ERR("Failed to insert texture!");
  }

  gridMap = map;
}

void RPGLayer::setGridDescriptor(std::shared_ptr<Graphic::EGridDesc> desc)
{
  gridDesc = desc;
}

void RPGLayer::testAnimation(Graphic::AnimationState state)
{
  std::shared_ptr<Graphic::EAnimation> ani;
  //for (auto& it : _sprite_map)
  for (auto& it : _object_map)
  {
    auto& object = it.second;
    switch (state) {
    case Graphic::ANI_STOP:
      object->stopAnimation();
      break;
    case Graphic::ANI_START:
      ani = std::shared_ptr<Graphic::EAnimation>(new Graphic::EGridMoveAnimation());
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
}

void RPGLayer::handleDirectonFactor(float axis_x, float axis_y)
{
  std::shared_ptr<Graphic::EAnimation> ani;
  Graphic::EGridMoveAnimation* grid = nullptr;

  for (auto& it : _object_map)
  {
    if (it.second->isControllable()) {
      auto& object = it.second;
      float obj_ax = axis_x;
      float obj_ay = axis_y;
      double x = object->getPositionX() / 32.0f;
      double y = object->getPositionY() / 32.0f;

      /* Check obstacle */
      if (0.0f < obj_ax) {
        /* Check right position */
        if (false == checkGridMoveable(int(x+1), int(y))) {
          obj_ax = 0.0f;
        }
      }
      if (obj_ax < 0.0f) {
        /* Check left position */
        if (false == checkGridMoveable(int(x-1), int(y))) {
          obj_ax = 0.0f;
        }
      }
      if (0.0f < obj_ay) {
        /* Check down position */
        if (false == checkGridMoveable(int(x), int(y+1))) {
          obj_ay = 0.0f;
        }
      }
      if (obj_ay < 0.0f) {
        /* Check up position */
        if (false == checkGridMoveable(int(x), int(y-1))) {
          obj_ay = 0.0f;
        }
      }

      /* Set factor */
      switch (object->getAnimationState())
      {
      case Graphic::ANI_NONE:
        if (obj_ax == 0.0f && obj_ay == 0.0f) {
          /* Ignore unexpected input on scene startup */
          break;
        }
        //LOG_DBG("Start animation. [%f / %f]", obj_ax, obj_ay);
        ani = std::shared_ptr<Graphic::EAnimation>(new Graphic::EGridMoveAnimation());
        ani->setCaller(object);
        grid = dynamic_cast<Graphic::EGridMoveAnimation*>(ani.get());
        grid->setAxisFactor(obj_ax, obj_ay);

        object->setAnimation(ani);
        object->startAnimation();
      break;
      case Graphic::ANI_START:
        /* Already in progress, Just update axis factor */
        ani = object->getAnimation();
        grid = dynamic_cast<Graphic::EGridMoveAnimation*>(ani.get());
        if (grid) {
          grid->setAxisFactor(obj_ax, obj_ay);
        }
      break;
      default:
        /* Already in progress */
        if ((std::isnan(obj_ax) || obj_ax == 0.0f) &&
          (std::isnan(obj_ay) || obj_ay == 0.0f))
        {
          /* Prevent character blocking state */
          return;
        }

        ani = object->getAnimation();
        grid = dynamic_cast<Graphic::EGridMoveAnimation*>(ani.get());
        if (grid) {
          grid->setAxisFactor(obj_ax, obj_ay);
        }
        object->startAnimation();
      break;
      }
    }
  }
}

/* TODO: Logic should be changed to register clickable position
  for each object */
bool RPGLayer::testRotate(int x, int y)
{
  bool event_consume = false;

  //for (auto& it : _sprite_map)
  for (auto& it : _object_map)
  {
    std::shared_ptr<Graphic::Object> object = it.second;
    if (nullptr == object) continue;

    int obj_x = object->getPositionX();
    int obj_y = object->getPositionY();
    int obj_x2 = object->getPositionX() + object->getWidth();
    int obj_y2 = object->getPositionY() + object->getHeight();

    if (obj_x <= x && x <= obj_x2 &&
      obj_y <= y && y <= obj_y2)
    {
      LOG_INFO("Rotate [%s]", object->getName().c_str());
      object->changeRotateState(object);

      event_consume = true;
      return true;
    }
  }

  return false;
}

bool RPGLayer::checkGridMoveable(int x, int y)
{
  bool res = false;

  if (gridDesc)
  {
    unsigned short a = gridDesc->getGridValue(0, x, y);
    if (0 == a)
      res = true;
  }

  return res;
}

void RPGLayer::objectPositionCallback(double x, double y)
{
  float ax = 0.0f, ay = 0.0f;

  /* TODO: Check obstacles */
  std::shared_ptr<Graphic::EAnimation> ani;
  Graphic::EGridMoveAnimation* grid = nullptr;

  for (auto& it : _object_map)
  {
    if (it.second->isControllable()) {
      auto& object = it.second;

      ani = object->getAnimation();
      grid = dynamic_cast<Graphic::EGridMoveAnimation*>(ani.get());
      if (nullptr == grid)
        continue;

      ax = grid->getAxisFactorX();
      ay = grid->getAxisFactorY();

      /* Check diagonal obstacle here */
      if ((ax == 0.0f) != (ay == 0.0f))
      {
        /* Only X or Y axis is mutually exclusive. */
        if (0.0f < ax) {
          /* Check right position */
          if (false == checkGridMoveable(int(x+1), int(y))) {
            handleDirectonFactor(0.0f, std::numeric_limits<float>::quiet_NaN());
          }
        }
        if (ax < 0.0f) {
          /* Check left position */
          if (false == checkGridMoveable(int(x-1), int(y))) {
            handleDirectonFactor(0.0f, std::numeric_limits<float>::quiet_NaN());
          }
        }
        if (0.0f < ay) {
          /* Check down position */
          if (false == checkGridMoveable(int(x), int(y+1))) {
            handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 0.0f);
          }
        }
        if (ay < 0.0f) {
          /* Check up position */
          if (false == checkGridMoveable(int(x), int(y-1))) {
            handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 0.0f);
          }
        }
      }
      else if ((ax != 0.0f) && (ay != 0.0f))
      {
        if (0.0f < ax) {
          /* Check right position */
          if (0.0f < ay) {
            /* Check right-down position */
            if (false == checkGridMoveable(int(x+1), int(y+1))) {
              ax = 0.0f;
              ay = 0.0f;
            }
          }
          if (ay < 0.0f) {
            /* Check right-up position */
            if (false == checkGridMoveable(int(x+1), int(y-1))) {
              ax = 0.0f;
              ay = 0.0f;
            }
          }
        }
        if (ax < 0.0f) {
          /* Check left position */
          if (0.0f < ay) {
            /* Check left-down position */
            if (false == checkGridMoveable(int(x-1), int(y+1))) {
              ax = 0.0f;
              ay = 0.0f;
            }
          }
          if (ay < 0.0f) {
            /* Check left-up position */
            if (false == checkGridMoveable(int(x-1), int(y-1))) {
              ax = 0.0f;
              ay = 0.0f;
            }
          }
        }
        handleDirectonFactor(ax, ay);
      }
    }
  }
}

void RPGLayer::handleEvent(SDL_Event e)
{
  /* Handler events for Scene instance */
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
    case SDLK_9:
      testAnimation(Graphic::ANI_START);
      break;
    case SDLK_0:
      testAnimation(Graphic::ANI_STOP);
      break;
    case SDLK_MINUS:
      testAnimation(Graphic::ANI_PAUSE);
      break;
    case SDLK_EQUALS:
      testAnimation(Graphic::ANI_RESUME);
      break;
#ifdef PLATFORM_IOS
    /* iCade Support */
    case SDLK_w:
      handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), -1.0f);
      break;
    case SDLK_x:
      handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 1.0f);
      break;
    case SDLK_a:
      handleDirectonFactor(-1.0f, std::numeric_limits<float>::quiet_NaN());
      break;
    case SDLK_d:
      handleDirectonFactor(1.0f, std::numeric_limits<float>::quiet_NaN());
      break;
    case SDLK_e:
      handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 0.0f);
    break;
    case SDLK_z:
      handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 0.0f);
    break;
    case SDLK_q:
      handleDirectonFactor(0.0f, std::numeric_limits<float>::quiet_NaN());
    break;
    case SDLK_c:
      handleDirectonFactor(0.0f, std::numeric_limits<float>::quiet_NaN());
    break;
#endif
    case SDLK_UP:
      //LOG_INFO("Move : UP");
      handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), -1.0f);
      break;
    case SDLK_DOWN:
      //LOG_INFO("Move : DOWN");
      handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 1.0f);
      break;
    case SDLK_LEFT:
      //LOG_INFO("Move : LEFT");
      handleDirectonFactor(-1.0f, std::numeric_limits<float>::quiet_NaN());
      break;
    case SDLK_RIGHT:
      //LOG_INFO("Move : RIGHT");
      handleDirectonFactor(1.0f, std::numeric_limits<float>::quiet_NaN());
      break;
    case SDLK_SPACE:
      LOG_INFO("Animation test");
      std::shared_ptr<Graphic::Object> found;
      auto search = _object_map.find("movingChar");
      if (search != _object_map.end()) {
        found = search->second;
        if (found->getAnimationState() != Graphic::ANI_START)
          found->changeState(found);
        LOG_DBG("Finished");
      } else {
        LOG_ERR("Object was not found !");
      }
      break;
    }
  }
  else if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {
    case SDLK_UP:
      //LOG_INFO("Key released : [UP]");
      handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 0.0f);
    break;
    case SDLK_DOWN:
      //LOG_INFO("Key released : [DOWN]");
      handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 0.0f);
    break;
    case SDLK_LEFT:
      //LOG_INFO("Key released : [LEFT]");
      handleDirectonFactor(0.0f, std::numeric_limits<float>::quiet_NaN());
    break;
    case SDLK_RIGHT:
      //LOG_INFO("Key released : [RIGHT]");
      handleDirectonFactor(0.0f, std::numeric_limits<float>::quiet_NaN());
    break;
    default: break;
    }
  } else if (e.type == SDL_MOUSEBUTTONDOWN) {
    /* TODO: event consumer should be handle more efficient way */
    bool consumed = testRotate(e.button.x, e.button.y);
    if (consumed)
      return;

    /* Move to clicked position */
    std::shared_ptr<Graphic::Object> found;
    auto search = _object_map.find("movingChar");
    if (search != _object_map.end()) {
      found = search->second;
      if (found->getAnimationState() != Graphic::ANI_START)
        found->animatedMoveTo(found, e.button.x, e.button.y, 1000);
    } else {
      LOG_ERR("Object was not found !");
    }
  } else if (e.type == SDL_FINGERMOTION) {
    SDL_TouchFingerEvent *te = &e.tfinger;
    int ax = te->dx * 1000;
    int ay = te->dy * 1000;

    if (ax <= -10) {
      handleDirectonFactor(-1.0f, 0.0f);
    } else if (10 <= ax) {
      handleDirectonFactor(1.0f, 0.0f);
    }

    if (ay <= -10) {
      handleDirectonFactor(0.0f, -1.0f);
    } else if (10 <= ay) {
      handleDirectonFactor(0.0f, 1.0f);
    }
  } else if (e.type == SDL_FINGERDOWN) {
    SDL_TouchFingerEvent *te = &e.tfinger;
    int x = 0, y = 0;

    x = story::Core::Ecore::getScreenWidth() * te->x;
    y = story::Core::Ecore::getScreenHeight() * te->y;

    /* TODO: event consumer should be handle more efficient way */
    bool consumed = testRotate(x, y);
    if (consumed)
      return;
  } else if (e.type == SDL_FINGERUP) {
    // SDL_TouchFingerEvent *te = &e.tfinger;

    /* Clear move direction */
    handleDirectonFactor(0.0f, 0.0f);
  }
}

void RPGLayer::render()
{
  if (gridMap)
    gridMap->render(0, 0, 0, 0, 0.0);

  for (auto &it : _img_texture_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _sprite_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _text_texture_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _drawable_map)
  {
    it.second->render(0, 0, 0, 0, 0.0);
  }

  for (auto& it : _object_map)
  {
    it.second->render();
  }
}

void RPGLayer::update(Uint32 currentTime, Uint32 accumulator)
{
  if (false == isActivated)
    return;

  for (auto& it : _sprite_map)
  {
    it.second->update(currentTime, accumulator);
  }

  for (auto& it : _text_texture_map)
  {
    it.second->update(currentTime, accumulator);
  }

  for (auto& it : _drawable_map)
  {
    it.second->update(currentTime, accumulator);
  }

  for (auto& it : _object_map)
  {
    it.second->update(currentTime, accumulator);
  }

  if (gridMap)
    gridMap->update(currentTime, accumulator);
}

} /* namespace Graphic */
} /* namespace story */
