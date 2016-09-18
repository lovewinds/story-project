#include "Ecore.hpp"
#include "util/LogHelper.hpp"

#include "texture/EGridMoveAnimation.hpp"

EGridMoveAnimation::EGridMoveAnimation()
{
}

EGridMoveAnimation::EGridMoveAnimation(GridMoveDir dir)
{
	direction = dir;
}

EGridMoveAnimation::~EGridMoveAnimation()
{
}

void EGridMoveAnimation::start()
{
	startTime = SDL_GetTicks();
	state = ANI_START;

	velo = 20.0;
	prevTime = 0;
}

void EGridMoveAnimation::stop()
{
	EAnimation::stop();
}

void EGridMoveAnimation::pause()
{
	elapsedTime = SDL_GetTicks() - startTime;
	state = ANI_PAUSE;
}

void EGridMoveAnimation::resume()
{
	startTime = SDL_GetTicks() - elapsedTime;
	state = ANI_START;
}

void EGridMoveAnimation::setDirection(GridMoveDir dir)
{
	direction = dir;
}

void EGridMoveAnimation::update(Uint32 currentTime, Uint32 accumulator)
{
	Uint32 compensatedTime = currentTime + accumulator;
	Uint32 atomicTime = (compensatedTime - startTime);
	Uint32 delta = atomicTime - prevTime;
	double dt = delta * 0.001;

	if (ANI_START != state)	return;

	prevTime = delta;

	/* Move position */
	switch (direction){
	case DIR_UP:
		a_y -= velo * dt;
		if (a_y <= -32.0) {
			velo = 0.0;
			a_y = -32.0;
			stop();
		}
		break;
	case DIR_DOWN:
		a_y += velo * dt;
		if (a_y >= 32.0) {
			velo = 0.0;
			a_y = 32.0;
			stop();
		}
		break;
	case DIR_LEFT:
		a_x -= velo * dt;
		if (a_x <= -32.0) {
			velo = 0.0;
			a_x = -32.0;
			stop();
		}
		break;
	case DIR_RIGHT:
		a_x += velo * dt;
		if (a_x >= 32.0) {
			velo = 0.0;
			a_x = 32.0;
			stop();
		}
		break;
	}
}