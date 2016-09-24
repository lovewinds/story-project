#include "Ecore.hpp"
#include "util/LogHelper.hpp"

#include "texture/EGridMoveAnimation.hpp"

#define TEST_VELO	16.0

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
	velo = TEST_VELO;
	prevTime = SDL_GetTicks();

	EAnimation::start();
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

void EGridMoveAnimation::setNextDirection(GridMoveDir dir)
{
	//LOG_DBG("Update next : [%x] -> [%x]", dir, next_dir);
	next_dir = dir;
}

void EGridMoveAnimation::update(Uint32 currentTime, Uint32 accumulator)
{
	Uint32 compensatedTime = currentTime + accumulator;
	/* Bug fix for iOS */
	Uint32 atomicTime = (prevTime==0) ? 0 : (
		(compensatedTime > prevTime) ? (compensatedTime - prevTime) : (prevTime - compensatedTime));
	Uint32 delta = atomicTime - prevTime;
	double dt = atomicTime * 0.01;
	double accu = 0.0;
	bool checkFinished = false;

	if (ANI_START != state)	return;

	//LOG_DBG("Time: %d / Prev: %d / dt: %lf", compensatedTime, prevTime, dt);

	prevTime = compensatedTime;

	/* Move position */
	switch (direction){
	case DIR_UP:
		a_y -= velo * dt;
		if (a_y <= -32.0) {
			if (next_dir != DIR_NONE && next_dir != direction) {
				accu = abs(a_y - (-32.0));
				a_y = -32.0;
			}
			checkFinished = true;
		}
		break;
	case DIR_DOWN:
		a_y += velo * dt;
		if (a_y >= 32.0) {
			if (next_dir != DIR_NONE && next_dir != direction) {
				accu = abs(a_y - (32.0));
				a_y = 32.0;
			}
			checkFinished = true;
		}
		break;
	case DIR_LEFT:
		a_x -= velo * dt;
		if (a_x <= -32.0) {
			if (next_dir != DIR_NONE && next_dir != direction) {
				accu = abs(a_x - (-32.0));
				a_x = -32.0;
			}
			checkFinished = true;
		}
		break;
	case DIR_RIGHT:
		a_x += velo * dt;
		if (a_x >= 32.0) {
			//LOG_DBG("Next right? [%s]", (next_dir == DIR_RIGHT) ? "True" : "False");
			if (next_dir != DIR_NONE && next_dir != direction) {
				accu = abs(a_x - (32.0));
				a_x = 32.0;
			}
			checkFinished = true;
		}
		break;
	}

	if (checkFinished) {
		if (DIR_NONE == next_dir) {
			LOG_DBG("Stopped");
			EAnimation::stop();
			a_x = 0;
			a_y = 0;
			startTime = 0;
			state = ANI_STOP;
		}
		else {
			EAnimation::sync();
			a_x = 0;
			a_y = 0;

			/* Reset state */
			velo = TEST_VELO;
			//prevTime = compensatedTime;
			direction = next_dir;
			next_dir = DIR_NONE;

			//EAnimation::start();
		}
	}
}