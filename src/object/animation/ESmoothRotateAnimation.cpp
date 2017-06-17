#include "Ecore.hpp"
#include "util/LogHelper.hpp"

#include "object/animation/ESmoothRotateAnimation.hpp"

ESmoothRotateAnimation::ESmoothRotateAnimation()
{
}

ESmoothRotateAnimation::~ESmoothRotateAnimation()
{
}

void ESmoothRotateAnimation::setRotateDirection(RotateDirection dir)
{
	direction = dir;
}

void ESmoothRotateAnimation::setTransition(int milliseconds)
{
	transition_msec = milliseconds;
}

#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))

void ESmoothRotateAnimation::update(Uint32 currentTime, Uint32 accumulator)
{
	Uint32 compensatedTime = currentTime + accumulator;

	/* Bug fix for iOS */
	Uint32 atomicTime = (prevTime==0) ? 0 : (
		(compensatedTime > prevTime) ? (compensatedTime - prevTime) : (prevTime - compensatedTime));
	Uint32 elapsed = compensatedTime - startTime;
	bool checkFinished = false;
	float v;
	float dt;

	if (ANI_START != state)	return;

	prevTime = compensatedTime;

	//a_x = ((float)start_x + ((float)(end_x - start_x) * ((float)elapsed / 1000.0f)));
	//a_y = ((float)start_y + ((float)(end_y - start_y) * ((float)elapsed / 1000.0f)));
/*
for (i = 0; i < N; i++) {
	v = i / N;
	v = SMOOTHSTEP(v);
	X = (A * v) + (B * (1 - v));
}
*/

	dt = ((float)elapsed) / (float)(transition_msec);
	if (dt >= 1.0f)
		checkFinished = true;

	v = SMOOTHSTEP(dt);
	if (direction == ROTATE_CLOCKWISE)
		a_angle = (360.0f * v) + (0.0f * (1 - v));
	else
		a_angle = (0.0f * v) + (360.0f * (1 - v));

	if (checkFinished) {
		LOG_DBG("Finished");
		EAnimation::stop();
		a_angle = 0.0f;
		startTime = 0;
		state = ANI_STOP;
	}
}