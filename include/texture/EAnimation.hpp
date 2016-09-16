#pragma once

#include <SDL.h>

enum AnimationState
{
	ANI_STOP = 0,
	ANI_START = 1,
	ANI_PAUSE = 2,
	ANI_RESUME = 3,
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

    double getX();
    double getY();

    virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
    Uint32 startTime;
    Uint32 elapsedTime;
    AnimationState state;

	/* Position */
	double a_x;
	double a_y;
};
