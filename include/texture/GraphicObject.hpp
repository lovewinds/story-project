#pragma once

#include "texture/EAnimation.hpp"
#include "texture/EDrawable.hpp"
#include "texture/ESprite.hpp"

namespace story {
namespace Graphic {

class Object
{
public:
	Object();
	Object(int x, int y);

	virtual ~Object();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

	/* Position */
	double getPositionX();
	double getPositionY();
	void movePositionTo(double x, double y);
	void movePositionBy(double delta_x, double delta_y);

	/* Animation */
	void setAnimation(std::shared_ptr<EAnimation> animation);
	std::shared_ptr<EAnimation> getAnimation();
	AnimationState getAnimationState();
	virtual void startAnimation();
	virtual void stopAnimation();
	virtual void pauseAnimation();
	virtual void resumeAnimation();
	virtual void finishedAnimationCallback(double delta_x, double delta_y);
	virtual void syncAnimationCallback(double delta_x, double delta_y);

	/* Primitive */
	std::string getName();
	void setVisible(bool visible);
	void setTransparent(int percent);
	void setSprite(std::shared_ptr<ESprite> sprite);

	/* compatibility */
	void setControllable(bool controllable);
	bool isControllable();

protected:
	std::string name;
	std::shared_ptr<EDrawable>	drawable;

	/* Animation */
	std::shared_ptr<EAnimation> animation;

	/* Position */
	double p_x;
	double p_y;

	/* */
	bool visible;
	int transparent;
	bool controllable;
};

} /* namespace Graphic */
} /* namespace story */
