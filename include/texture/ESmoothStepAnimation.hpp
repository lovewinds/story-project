#pragma once

#include "texture/EAnimation.hpp"

class ESmoothStepAnimation : public EAnimation
{
public:
	ESmoothStepAnimation();
	virtual ~ESmoothStepAnimation();

	virtual void start();
	virtual void stop();
	virtual void pause();
	virtual void resume();

	virtual void setStartPosition(int x, int y);
	virtual void setEndPosition(int x, int y);
	virtual void setTransition(int seconds);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
	Uint32 prevTime = 0;

	int start_x = 0;
	int start_y = 0;
	int end_x = 0;
	int end_y = 0;
	int transition_sec = 0;
};
