#include "Ecore.hpp"
#include "util/LogHelper.hpp"

#include "texture/EAccelAnimation.hpp"

EAccelAnimation::EAccelAnimation()
{
	decrease = false;
}

EAccelAnimation::~EAccelAnimation()
{
}

void EAccelAnimation::start()
{
	startTime = SDL_GetTicks();
	state = ANI_START;

	velo = 10.0;
	accel = 0.0;
	prevTime = 0;
	decrease = false;
}

void EAccelAnimation::stop()
{
	startTime = 0;
	state = ANI_STOP;
}

void EAccelAnimation::pause()
{
	elapsedTime = SDL_GetTicks() - startTime;
	state = ANI_PAUSE;
}

void EAccelAnimation::resume()
{
	startTime = SDL_GetTicks() - elapsedTime;
	state = ANI_START;
}

void EAccelAnimation::update(Uint32 currentTime, Uint32 accumulator)
{
	Uint32 compensatedTime = currentTime + accumulator;
	Uint32 atomicTime = (compensatedTime - startTime);
	Uint32 delta = atomicTime - prevTime;
	double dt = delta * 0.001;

	if (atomicTime > 300) {
		decrease = true;
		accel = -0.25;
	}

	if (ANI_START != state)	return;

	prevTime = delta;

	if (decrease) {
		velo += accel * dt;
	}

	/* Move position */
	a_x += velo * dt;

	/* End animation */
	if (velo <= 0.0) {
		velo = 0.0;
		accel = 0.0;
		stop();
	}
}