#pragma once

#include <string>
#include <memory>
#include <map>
#include <functional>

#include "graphic/animation/EAnimation.hpp"
#include "graphic/texture/EDrawable.hpp"
#include "graphic/texture/ESprite.hpp"
#include "graphic/texture/EImageTexture.hpp"
#include "graphic/texture/ETextTexture.hpp"

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
	int getWidth();
	int getHeight();

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

	virtual void animatedMoveTo(std::weak_ptr<story::Graphic::Object> self,
			int x, int y, int transition_msec);
	virtual void animatedMoveRotateTo(std::weak_ptr<story::Graphic::Object> self,
			int dest_x, int dest_y, int transition_msec);

	/* State */
	void changeState(std::weak_ptr<story::Graphic::Object> self);
	void changeRotateState(std::weak_ptr<story::Graphic::Object> self);

	/* Event callback */
	typedef std::function<void(double x, double y)> PositionCallback;
	void setPositionCallback(PositionCallback cb);

	/* Primitive */
	std::string getName();
	void setName(std::string name);
	void setVisible(bool visible);
	void setTransparent(int percent);

	void addSprite(std::shared_ptr<ESprite> sprite);
	void addImage(std::shared_ptr<EImageTexture> image);
	void addText(std::shared_ptr<ETextTexture> text);
	void updateText(std::string text);
	void removeContentAll();

	/* compatibility */
	void setControllable(bool controllable);
	bool isControllable();

protected:
	std::string name;

	std::map<std::string, std::shared_ptr<ESprite>> _sprite_map;
	std::map<std::string, std::shared_ptr<EImageTexture>> _img_texture_map;
	std::map<std::string, std::shared_ptr<ETextTexture>> _text_texture_map;

	/* Animation */
	std::shared_ptr<EAnimation> animation;

	/* Position */
	double p_x;
	double p_y;

	int width;
	int height;

	/* */
	bool visible;
	int transparent;
	bool controllable;

	int state_value;
	int rotate_state_value;

	/* Callback */
	PositionCallback callback;
};

} /* namespace Graphic */
} /* namespace story */