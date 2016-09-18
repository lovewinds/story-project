#pragma once

#include "texture/EAnimation.hpp"

class EAccelAnimation : public EAnimation
{
public:
	EAccelAnimation();
	virtual ~EAccelAnimation();

	virtual void start();
	virtual void stop();
	virtual void pause();
	virtual void resume();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
	bool decrease;
	double accel = 0.026;
	double velo = 1.0;
	Uint32 prevTime = 0;
};