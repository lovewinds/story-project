#include "Ecore.hpp"
#include "util/LogHelper.hpp"

#include "texture/ESmoothStepAnimation.hpp"

ESmoothStepAnimation::ESmoothStepAnimation()
{
}

ESmoothStepAnimation::~ESmoothStepAnimation()
{
}

void ESmoothStepAnimation::start()
{
	prevTime = SDL_GetTicks();

	EAnimation::start();
}

void ESmoothStepAnimation::stop()
{
	EAnimation::stop();
}

void ESmoothStepAnimation::pause()
{
	elapsedTime = SDL_GetTicks() - startTime;
	state = ANI_PAUSE;
}

void ESmoothStepAnimation::resume()
{
	startTime = SDL_GetTicks() - elapsedTime;
	state = ANI_START;
}

void ESmoothStepAnimation::setStartPosition(int x, int y)
{
	start_x = x;
	start_y = y;
}

void ESmoothStepAnimation::setEndPosition(int x, int y)
{
	end_x = x;
	end_y = y;
}

void ESmoothStepAnimation::setTransition(int seconds)
{
	transition_sec = seconds;
}

#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))

void ESmoothStepAnimation::update(Uint32 currentTime, Uint32 accumulator)
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

	dt = ((float)elapsed) / (transition_sec * 1000.0f);
	if (dt >= 1.0f)
		checkFinished = true;

    v = SMOOTHSTEP(dt);
    a_x = (end_x * v) + (start_x * (1 - v));
    a_y = (end_y * v) + (start_y * (1 - v));
#if 0
	LOG_DBG("Elapsed: %d(dt:%f) | [%d, %d]=>[%d, %d]=>[%d, %d]",
			elapsed, dt,
			start_x, start_y,
			(int)a_x, (int)a_y,
			end_x, end_y);
#endif
#if 0
	if (start_x < end_x) {
		if (start_y < end_y) {
			// right down
			if (end_x <= a_x && end_y <= a_y)	checkFinished = true;
		} else {
			// right up
			if (end_x <= a_x && a_y <= end_y)	checkFinished = true;
		}
	} else {
		if (start_y < end_y) {
			// left down
			if (a_x <= end_x && end_y <= a_y)	checkFinished = true;
		} else {
			// left up
			if (a_x <= end_x && a_y <= end_y)	checkFinished = true;
		}
	}
#endif

	if (checkFinished) {
		LOG_DBG("Finished");
		EAnimation::stop();
		a_x = 0;
		a_y = 0;
		startTime = 0;
		state = ANI_STOP;
	}
}