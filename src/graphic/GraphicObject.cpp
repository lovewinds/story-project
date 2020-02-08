#include "core/Ecore.hpp"

#include "graphic/GraphicObject.hpp"
#include "graphic/animation/ESmoothStepAnimation.hpp"
#include "graphic/animation/ESmoothRotateAnimation.hpp"
#include "graphic/animation/ERotateAndMoveAnimation.hpp"

#include "util/LogHelper.hpp"

namespace story {
namespace Graphic {

const double Object::DEFAULT_ACCEL = 0.026;
const double Object::DEFAULT_VELO = 0.2;

Object::Object() :
p_x(0), p_y(0),
movement_start_time(0.0), movement_prev_time(0.0),
movement_state_x(0.0), movement_state_y(0.0),
width(0), height(0)
{
  /* Initialize */
  visible = true;
  transparent = 100;

  state_value = 0;
  rotate_state_value = 0;

  controllable = false;

  movement_state_x = std::numeric_limits<double>::quiet_NaN();
  movement_state_y = std::numeric_limits<double>::quiet_NaN();
  movement_accel_x = 0.0;
  movement_accel_y = 0.0;
  movement_velo_x = 0.0;
  movement_velo_y = 0.0;
}

Object::~Object()
{
}

void Object::setMovement(double x, double y)
{
  // Ignore simple touching
  if (std::isnan(movement_state_x) && std::isnan(movement_state_y) &&
      0.0 == x && 0.0 == y)
    return;

  if (0.0 == movement_start_time) {
    movement_prev_time = movement_start_time = Core::Ecore::getAppTicks();
  }

  // State :: nan -> 1.0 -> 0.0 -> nan
  //                start   stop

  if (std::isnan(x) == false) {
    movement_state_x = x;
    // Start
    if (movement_state_x != 0.0)
      movement_velo_x = movement_state_x * DEFAULT_VELO;
    else {
      // Stop
      movement_accel_x = DEFAULT_ACCEL;
    }
  } 
  if (std::isnan(y) == false) {
    movement_state_y = y;
    // Start
    if (movement_state_y != 0.0)
      movement_velo_y = movement_state_y * DEFAULT_VELO;
    else {
      // Stop
      movement_accel_y = DEFAULT_ACCEL;
    }
  }
}

void Object::movePositionTo(double x, double y)
{
  for (auto &it : _sprite_map)
  {
    it.second->movePositionTo(x, y);
  }

  for (auto &it : _img_texture_map)
  {
    it.second->movePositionTo(x, y);
  }

  p_x = x;
  p_y = y;
}

void Object::movePositionBy(double delta_x, double delta_y)
{
  for (auto &it : _sprite_map)
  {
    it.second->movePositionBy(delta_x, delta_y);
  }

  for (auto &it : _img_texture_map)
  {
    it.second->movePositionBy(delta_x, delta_y);
  }

  p_x += delta_x;
  p_y += delta_y;
}

double Object::getPositionX()
{
  return p_x;
}

double Object::getPositionY()
{
  return p_y;
}

int Object::getWidth()
{
  return width;
}

int Object::getHeight()
{
  return height;
}

std::string Object::getName()
{
  return name;
}

void Object::setName(std::string name)
{
  this->name = name;
}

void Object::setVisible(bool visible)
{
  this->visible = visible;
}

void Object::setTransparent(int percent)
{
  this->transparent = percent;
}

void Object::setControllable(bool controllable)
{
  this->controllable = controllable;
}

bool Object::isControllable()
{
  return controllable;
}

void Object::addSprite(std::shared_ptr<ESprite> sprite)
{
  auto result = _sprite_map.emplace(sprite->getName(), sprite);
  if (!result.second) {
    LOG_ERR("Failed to insert sprite map!");
    return;
  }

  sprite->movePositionTo(p_x, p_y);

  /* TODO: support multiple drawbles */
  width = sprite->getWidth();
  height = sprite->getHeight();
}

void Object::addImage(std::shared_ptr<EImageTexture> image)
{
  auto result = _img_texture_map.emplace(image->getName(), image);
  if (!result.second) {
    LOG_ERR("Failed to insert image map!");
    return;
  }

  image->movePositionTo(p_x, p_y);

  /* TODO: support multiple drawbles */
  width = image->getWidth();
  height = image->getHeight();
}

void Object::addText(std::shared_ptr<ETextTexture> text)
{
  auto result = _text_texture_map.emplace("text", text);
  if (!result.second) {
    LOG_ERR("Failed to insert text map!");
    return;
  }

  text->movePositionTo(p_x, p_y);

  /* TODO: support multiple drawbles */
  width = text->getWidth();
  height = text->getHeight();
}

void Object::updateText(std::string text)
{
  for (auto& it : _text_texture_map) {
    it.second->setText(text);
  }
}

void Object::removeContentAll()
{
  _sprite_map.clear();
  _img_texture_map.clear();
  _text_texture_map.clear();
}

void Object::setAnimation(std::shared_ptr<EAnimation> animation)
{
  this->animation = animation;
}

std::shared_ptr<EAnimation> Object::getAnimation()
{
  return animation;
}

void Object::animatedMoveTo(std::weak_ptr<story::Graphic::Object> self,
    int dest_x, int dest_y, int transition_msec)
{
  /* Create animation */
  ESmoothStepAnimation *ani = new ESmoothStepAnimation();
  ani->setCaller(self);
  ani->setStartPosition(0, 0);
  ani->setEndPosition(dest_x - p_x, dest_y - p_y);
  ani->setTransition(transition_msec);

  this->setAnimation(std::shared_ptr<EAnimation>(ani));
  this->startAnimation();
}

void Object::animatedMoveRotateTo(std::weak_ptr<story::Graphic::Object> self,
    int dest_x, int dest_y, int transition_msec)
{
  int i = 0;
  /* Create animation */
  ERotateAndMoveAnimation *ani = new ERotateAndMoveAnimation();
  ani->setCaller(self);
  ani->setStartPosition(0, 0);
  ani->setEndPosition(dest_x - p_x, dest_y - p_y);
  ani->setTransition(transition_msec);
  i = rand() % 2;
  if (i == 0)
    ani->setRotateDirection(ROTATE_CLOCKWISE);
  else
    ani->setRotateDirection(ROTATE_ANTICLOCKWISE);

  this->setAnimation(std::shared_ptr<EAnimation>(ani));
  this->startAnimation();
}

void Object::changeState(std::weak_ptr<story::Graphic::Object> self)
{
  /* Initial state */
  if (this->state_value == 0) {
    /* Create animation */
    ESmoothStepAnimation *ani = new ESmoothStepAnimation();
    ani->setCaller(self);
    ani->setStartPosition(10, 0);
    ani->setEndPosition(100, 0);
    ani->setTransition(1000);

    this->setAnimation(std::shared_ptr<EAnimation>(ani));
    this->startAnimation();

    this->state_value = 1;
  } else {
    /* Create animation */
    ESmoothStepAnimation *ani = new ESmoothStepAnimation();
    ani->setCaller(self);
    ani->setStartPosition(10, 0);
    ani->setEndPosition(100, 0);
    ani->setTransition(1000);

    this->setAnimation(std::shared_ptr<EAnimation>(ani));
    this->startAnimation();

    this->state_value = 0;
  }
}

void Object::changeRotateState(std::weak_ptr<story::Graphic::Object> self)
{
  /* Initial state */
  switch(this->rotate_state_value) {
  case 0: {
    /* Create animation */
    ESmoothRotateAnimation *ani = new ESmoothRotateAnimation();
    ani->setCaller(self);
    ani->setRotateDirection(ROTATE_CLOCKWISE);
    ani->setTransition(1000);

    this->setAnimation(std::shared_ptr<EAnimation>(ani));
    this->startAnimation();

    this->rotate_state_value = 1;
  }
  break;
  case 1: {
    /* Create animation */
    ESmoothRotateAnimation *ani = new ESmoothRotateAnimation();
    ani->setCaller(self);
    ani->setRotateDirection(ROTATE_ANTICLOCKWISE);
    ani->setTransition(1000);

    this->setAnimation(std::shared_ptr<EAnimation>(ani));
    this->startAnimation();

    this->rotate_state_value = 0;
  }
  break;
  default:
    this->rotate_state_value = 0;
  break;
  }
}

AnimationState Object::getAnimationState()
{
  if (nullptr == animation)
    return ANI_NONE;

  return animation->getState();
}

void Object::startAnimation()
{
  if (animation) {
    animation->start();
  }
}

void Object::stopAnimation()
{
  if (animation) {
    animation->stop();
  }
}

void Object::pauseAnimation()
{
  if (animation) {
    animation->pause();
  }
}

void Object::resumeAnimation()
{
  if (animation) {
    animation->resume();
  }
}

void Object::finishedAnimationCallback(double ani_x, double ani_y)
{
  //LOG_DBG("Animation Finished. update position [%lf, %lf]", ani_x, ani_y);

  for (auto &it : _img_texture_map)
  {
    it.second->movePositionBy(ani_x, ani_y);
  }

  for (auto& it : _sprite_map)
  {
    it.second->movePositionBy(ani_x, ani_y);
  }

  for (auto& it : _text_texture_map)
  {
    it.second->movePositionBy(ani_x, ani_y);
  }

  p_x += ani_x;
  p_y += ani_y;

  //LOG_DBG("Current pos: [%lf, %lf]", p_x / 32.0f, p_y / 32.0f);

  if (callback)
    callback(p_x / 32.0f, p_y / 32.0f);
}

void Object::syncAnimationCallback(double ani_x, double ani_y)
{
  for (auto &it : _img_texture_map)
  {
    it.second->movePositionBy(ani_x, ani_y);
  }

  for (auto& it : _sprite_map)
  {
    it.second->movePositionBy(ani_x, ani_y);
  }

  for (auto& it : _text_texture_map)
  {
    it.second->movePositionBy(ani_x, ani_y);
  }

  p_x += ani_x;
  p_y += ani_y;

  //LOG_DBG("Current pos: [%lf, %lf]", p_x / 32.0f, p_y / 32.0f);

  if (callback)
    callback(p_x / 32.0f, p_y / 32.0f);
}

void Object::calculateMovement(Uint32 currentTime, Uint32 accumulator)
{
  // default : 0.026
  // static double movement_accel_x = 0.0, movement_accel_y = 0.0;
  // static double movement_velo_x = 0.01, movement_velo_y = 0.01;

  double movement_x = 0.0;
  double movement_y = 0.0;
  Uint32 atomicTime = currentTime - movement_start_time;
  Uint32 deltaTime = currentTime - movement_prev_time;

  // Decresement
  if (0.0 == movement_state_x) {
    movement_velo_x = movement_accel_x * deltaTime * 0.001;
    // baseline
    if (movement_accel_x > 0.0 && movement_velo_x < 0.00005) {
      movement_velo_x = 0.0;
      movement_state_x = std::numeric_limits<double>::quiet_NaN();
    }
    if (movement_accel_x < 0.0 && movement_velo_x > 0.00005) {
      movement_velo_x = 0.0;
      movement_state_x = std::numeric_limits<double>::quiet_NaN();
    }
  }
  if (0.0 == movement_state_y) {
    movement_velo_y = movement_accel_y * deltaTime * 0.001;
    // baseline
    if (movement_accel_y > 0.0 && movement_velo_y < 0.00005) {
      movement_velo_y = 0.0;
      movement_state_y = std::numeric_limits<double>::quiet_NaN();
    }
    if (movement_accel_y < 0.0 && movement_velo_y > 0.00005) {
      movement_velo_y = 0.0;
      movement_state_y = std::numeric_limits<double>::quiet_NaN();
    }
  }

  // Calculate delta movement
  movement_x = movement_velo_x * deltaTime;
  movement_y = movement_velo_y * deltaTime;
  movement_prev_time = currentTime;

  // Move actual object
  p_x += movement_x;
  p_y += movement_y;
  // LOG_DBG("  [Delta %04d] POS :: [%0.2f, %0.2f] || v [%0.4f %0.4f] || a [%0.4f %0.f]",
  //     deltaTime,
  //     p_x, p_y,
  //     movement_velo_x, movement_velo_y,
  //     movement_accel_x, movement_accel_y);
  if (0.0 == movement_velo_x && 0.0 == movement_velo_y) {
    movement_start_time = 0.0;
    // LOG_INFO("    RESET time :: %u [%0.2f %0.2f]",
    //   movement_start_time,
    //   movement_state_x, movement_state_y);
  }
}

void Object::setPositionCallback(PositionCallback cb)
{
  callback = std::move(cb);
}

void Object::update(Uint32 currentTime, Uint32 accumulator)
{
  for (auto &it : _img_texture_map)
  {
    it.second->update(currentTime, accumulator);
  }

  for (auto& it : _sprite_map)
  {
    it.second->update(currentTime, accumulator);
  }

  for (auto& it : _text_texture_map)
  {
    it.second->update(currentTime, accumulator);
  }

  /* Animation */
  if (animation) {
    animation->update(currentTime, accumulator);
  }

  if (0.0 != movement_velo_x || 0.0 != movement_velo_y)
    calculateMovement(currentTime, accumulator);
}

void Object::render()
{
  double ani_x = 0.0f;
  double ani_y = 0.0f;
  double ani_rotate = 0.0f;

  if (animation) {
    ani_x = animation->getX();
    ani_y = animation->getY();
    ani_rotate = animation->getAngle();
  }

  for (auto &it : _img_texture_map)
  {
    it.second->render(p_x, p_y, ani_x, ani_y, ani_rotate);
  }

  for (auto& it : _sprite_map)
  {
    it.second->render(p_x, p_y, ani_x, ani_y, ani_rotate);
  }

  for (auto& it : _text_texture_map)
  {
    it.second->render(p_x, p_y, ani_x, ani_y, ani_rotate);
  }

}


} /* namespace Graphic */
} /* namespace story */
