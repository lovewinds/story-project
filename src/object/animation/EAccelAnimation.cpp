#include "Ecore.hpp"
#include "util/LogHelper.hpp"

#include "object/animation/EAccelAnimation.hpp"

EAccelAnimation::EAccelAnimation()
{
	decrease = false;
}

EAccelAnimation::~EAccelAnimation()
{
}

void EAccelAnimation::start()
{
	startTime = Ecore::getAppTicks();
	state = ANI_START;

	velo = 10.0;
	accel = 0.0;
	prevTime = 0;
	decrease = false;
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