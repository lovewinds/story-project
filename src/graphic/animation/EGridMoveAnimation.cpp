#include <cmath>

#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"

#include "graphic/animation/EGridMoveAnimation.hpp"

#define TEST_VELO	(16.0f)
#define GRID_SIZE	(32.0f)

namespace story {
namespace Graphic {

EGridMoveAnimation::EGridMoveAnimation()
{
}

EGridMoveAnimation::~EGridMoveAnimation()
{
}

void EGridMoveAnimation::start()
{
	velo = TEST_VELO;
	EAnimation::start();

	curr_dir_factor_x = next_dir_factor_x;
	curr_dir_factor_y = next_dir_factor_y;
}

void EGridMoveAnimation::setAxisFactor(float axis_x, float axis_y)
{
	if (std::isnan(axis_x) == false)
		next_dir_factor_x = axis_x;
	if (std::isnan(axis_y) == false)
		next_dir_factor_y = axis_y;
}

float EGridMoveAnimation::getAxisFactorX()
{
	return next_dir_factor_x;
}

float EGridMoveAnimation::getAxisFactorY()
{
	return next_dir_factor_y;
}

void EGridMoveAnimation::update(Uint32 currentTime, Uint32 accumulator)
{
	Uint32 compensatedTime = currentTime + accumulator;
	/* Bug fix for iOS */
	Uint32 atomicTime = (prevTime==0) ? 0 : (
		(compensatedTime > prevTime) ? (compensatedTime - prevTime) : (prevTime - compensatedTime));
	// Uint32 delta = atomicTime - prevTime;
	double dt = atomicTime * 0.01;
	bool checkFinished = false;

	if (ANI_START != state)	return;

	prevTime = compensatedTime;

	a_x += velo * dt * curr_dir_factor_x;
	a_y += velo * dt * curr_dir_factor_y;

	if (a_x <= -GRID_SIZE) {
		a_x = -GRID_SIZE;
		checkFinished = true;
	}
	if (a_x >= GRID_SIZE) {
		a_x = GRID_SIZE;
		checkFinished = true;
	}
	if (a_y <= -GRID_SIZE) {
		a_y = -GRID_SIZE;
		checkFinished = true;
	}
	if (a_y >= GRID_SIZE) {
		a_y = GRID_SIZE;
		checkFinished = true;
	}

	if (checkFinished) {
		if (next_dir_factor_x > 0.0f || next_dir_factor_x < 0.0f
			|| next_dir_factor_y > 0.0f || next_dir_factor_y < 0.0f)
		{
			EAnimation::sync();
			a_x = 0.0f;
			a_y = 0.0f;
			curr_dir_factor_x = next_dir_factor_x;
			curr_dir_factor_y = next_dir_factor_y;

			if (next_dir_factor_x == 0.0f && next_dir_factor_y == 0.0f) {
				EAnimation::stop();
				startTime = 0;
				prevTime = 0;
				state = ANI_STOP;
			}

			/* Reset state */
			//prevTime = compensatedTime;
			//EAnimation::start();
		} else {
			EAnimation::stop();
			a_x = 0.0f;
			a_y = 0.0f;
			curr_dir_factor_x = next_dir_factor_x;
			curr_dir_factor_y = next_dir_factor_y;
			startTime = 0;
			prevTime = 0;
			state = ANI_STOP;
		}
	}
}
#if 0
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
#endif

} /* namespace Graphic */
} /* namespace story */
