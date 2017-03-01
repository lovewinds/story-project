#pragma once

#include "texture/EAnimation.hpp"

enum RotateDirection {
	ROTATE_CLOCKWISE,
	ROTATE_ANTICLOCKWISE
};

class ESmoothRotateAnimation : public EAnimation
{
public:
	ESmoothRotateAnimation();
	virtual ~ESmoothRotateAnimation();

	virtual void start();
	virtual void stop();
	virtual void pause();
	virtual void resume();

	virtual void setRotateDirection(RotateDirection dir);
	virtual void setTransition(int milliseconds);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
	Uint32 prevTime = 0;

	int transition_msec = 1000;

	RotateDirection direction;
};
