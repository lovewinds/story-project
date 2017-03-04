#pragma once

#include "texture/EAnimation.hpp"

class ESmoothRotateAnimation : public EAnimation
{
public:
	ESmoothRotateAnimation();
	virtual ~ESmoothRotateAnimation();

	virtual void setRotateDirection(RotateDirection dir);
	virtual void setTransition(int milliseconds);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
	int transition_msec = 1000;

	RotateDirection direction;
};
