#include "Ecore.hpp"
#include "texture/GraphicObject.hpp"
#include "util/LogHelper.hpp"

namespace story {
namespace Graphic {

Object::Object() :
p_x(0), p_y(0)
{
	/* Initialize */
	visible = true;
	transparent = 100;
}

Object::~Object()
{
}

void Object::movePositionTo(double x, double y)
{
	if (drawable)
		drawable->movePositionTo(x, y);
}

void Object::movePositionBy(double delta_x, double delta_y)
{
	if (drawable)
		drawable->movePositionBy(delta_x, delta_y);
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

void Object::setSprite(std::shared_ptr<ESprite> sprite)
{
	this->drawable = std::dynamic_pointer_cast<EDrawable>(sprite);
}

void Object::setAnimation(std::shared_ptr<EAnimation> animation)
{
	this->animation = animation;
}

std::shared_ptr<EAnimation> Object::getAnimation()
{
	return animation;
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
	LOG_DBG("Animation Finished. update position");
	p_x += delta_x;
	p_y += delta_y;

	//animation = nullptr;
}

void Object::syncAnimationCallback(double delta_x, double delta_y)
{
	LOG_DBG("update position");
	p_x += delta_x;
	p_y += delta_y;
}

void Object::update(Uint32 currentTime, Uint32 accumulator)
{
	if (drawable) {
		drawable->update(currentTime, accumulator);
	}

	/* Animation */
	if (animation) {
		animation->update(currentTime, accumulator);
	}
}

void Object::render()
{
	if (drawable) {
		drawable->render(0, 0);
	}
}


} /* namespace Graphic */
} /* namespace story */
