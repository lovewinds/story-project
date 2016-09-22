#pragma once

#include "texture/EAnimation.hpp"

enum GridMoveDir {
	DIR_NONE,
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
};

class EGridMoveAnimation : public EAnimation
{
public:
	EGridMoveAnimation();
	EGridMoveAnimation(GridMoveDir dir);
	virtual ~EGridMoveAnimation();

	virtual void start();
	virtual void stop();
	virtual void pause();
	virtual void resume();

	virtual void setDirection(GridMoveDir dir);
	virtual void setNextDirection(GridMoveDir dir);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
	double accel = 0.026;
	double velo = 10.0;
	Uint32 prevTime = 0;

	GridMoveDir direction = DIR_NONE;
	GridMoveDir next_dir = DIR_NONE;
};
