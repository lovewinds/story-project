#include "Ecore.hpp"

#include "texture/GraphicObject.hpp"
#include "texture/ELerpAnimation.hpp"

#include "util/LogHelper.hpp"

namespace story {
namespace Graphic {

Object::Object() :
p_x(0), p_y(0)
{
	/* Initialize */
	visible = true;
	transparent = 100;
	state_value = 0;
	controllable = false;
}

Object::~Object()
{
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
}

void Object::addImage(std::shared_ptr<EImageTexture> image)
{
	auto result = _img_texture_map.emplace(image->getName(), image);
	if (!result.second) {
		LOG_ERR("Failed to insert image map!");
		return;
	}

	image->movePositionTo(p_x, p_y);
}

void Object::setAnimation(std::shared_ptr<EAnimation> animation)
{
	this->animation = animation;
}

std::shared_ptr<EAnimation> Object::getAnimation()
{
	return animation;
}

void Object::moveWithAnimation(std::weak_ptr<story::Graphic::Object> self, int x, int y)
{
	/* Create animation */
	ELerpAnimation *ani = new ELerpAnimation();
	ani->setCaller(self);
	ani->setStartPosition(p_x, p_y);
	ani->setEndPosition(x, y);

	this->setAnimation(std::shared_ptr<EAnimation>(ani));
	this->startAnimation();
}

void Object::changeState(std::weak_ptr<story::Graphic::Object> self)
{
	/* Initial state */
	if (this->state_value == 0) {
		/* Create animation */
		ELerpAnimation *ani = new ELerpAnimation();
		ani->setCaller(self);
		ani->setStartPosition(10, 0);
		ani->setEndPosition(100, 0);

		this->setAnimation(std::shared_ptr<EAnimation>(ani));
		this->startAnimation();

		this->state_value = 1;
	} else {
		/* Create animation */
		ELerpAnimation *ani = new ELerpAnimation();
		ani->setCaller(self);
		ani->setStartPosition(10, 0);
		ani->setEndPosition(100, 0);

		this->setAnimation(std::shared_ptr<EAnimation>(ani));
		this->startAnimation();

		this->state_value = 0;
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

void Object::finishedAnimationCallback(double delta_x, double delta_y)
{
	LOG_DBG("Animation Finished. update position [%lf, %lf]", delta_x, delta_y);

	for (auto &it : _img_texture_map)
	{
		it.second->movePositionBy(delta_x, delta_y);
	}

	for (auto& it : _sprite_map)
	{
		it.second->movePositionBy(delta_x, delta_y);
	}

	//animation = nullptr;
}

void Object::syncAnimationCallback(double delta_x, double delta_y)
{
	LOG_DBG("update position [%lf, %lf]", delta_x, delta_y);

	for (auto &it : _img_texture_map)
	{
		it.second->movePositionBy(delta_x, delta_y);
	}

	for (auto& it : _sprite_map)
	{
		it.second->movePositionBy(delta_x, delta_y);
	}
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

	/* Animation */
	if (animation) {
		animation->update(currentTime, accumulator);
	}
}

void Object::render()
{
	double ani_x = 0.0f;
	double ani_y = 0.0f;

	if (animation) {
		ani_x = animation->getX();
		ani_y = animation->getY();
	}

	for (auto &it : _img_texture_map)
	{
		it.second->render(ani_x, ani_y);
	}

	for (auto& it : _sprite_map)
	{
		it.second->render(ani_x, ani_y);
	}
}


} /* namespace Graphic */
} /* namespace story */
