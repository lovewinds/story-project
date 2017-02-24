#pragma once

#include <string>
#include <memory>
#include <map>

#include "texture/EAnimation.hpp"
#include "texture/EDrawable.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"

namespace story {
namespace Graphic {

class Object
{
public:
	Object();

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

	/* State */
	void changeState(std::weak_ptr<story::Graphic::Object> self);

	/* Primitive */
	std::string getName();
	void setName(std::string name);
	void setVisible(bool visible);
	void setTransparent(int percent);
	void addSprite(std::shared_ptr<ESprite> sprite);
	void addImage(std::shared_ptr<EImageTexture> sprite);

	/* compatibility */
	void setControllable(bool controllable);
	bool isControllable();

protected:
	std::string name;

	std::map<std::string, std::shared_ptr<ESprite>> _sprite_map;
	std::map<std::string, std::shared_ptr<EImageTexture>> _img_texture_map;

	/* Animation */
	std::shared_ptr<EAnimation> animation;

	/* Position */
	double p_x;
	double p_y;

	/* */
	bool visible;
	int transparent;
	bool controllable;

	int state_value;
};

} /* namespace Graphic */
} /* namespace story */
