#pragma once

#include <iostream>
#include <memory>

//#include "texture/GraphicObject.hpp"

/* Avoid circular reference */
//class EDrawable;
namespace story {
	namespace Graphic {
		class Object;
	}
}

enum AnimationRepeatType
{
	ANI_NO_REPEAT,
	ANI_REPEAT
};

enum AnimationState
{
	ANI_NONE,
	ANI_STOP,
	ANI_START,
	ANI_PAUSE,
	ANI_RESUME,
};

enum RotateDirection {
	ROTATE_CLOCKWISE,
	ROTATE_ANTICLOCKWISE
};

class EAnimation
{
public:
	EAnimation();
	virtual ~EAnimation();

	virtual void start();
	virtual void stop();
    virtual void pause();
    virtual void resume();
	virtual void sync();

    double getX();
    double getY();
	double getAngle();
	AnimationState getState();

	void setCaller(std::weak_ptr<story::Graphic::Object> caller);

    virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
    Uint32 startTime;
    Uint32 elapsedTime;
    AnimationState state;

	/* This model only supports one callback at one time. */
	/* TODO: Use callback list */
	//std::weak_ptr<story::Graphic::Object> caller;

	std::weak_ptr<story::Graphic::Object> caller;

	/* Position */
	double a_x;
	double a_y;
	double a_angle;
};
