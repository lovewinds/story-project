#include "Ecore.hpp"
#include "util/LogHelper.hpp"

#include "texture/ELerpAnimation.hpp"

ELerpAnimation::ELerpAnimation()
{
}

ELerpAnimation::~ELerpAnimation()
{
}

void ELerpAnimation::start()
{
	prevTime = SDL_GetTicks();

	EAnimation::start();
}

void ELerpAnimation::stop()
{
	EAnimation::stop();
}

void ELerpAnimation::pause()
{
	elapsedTime = SDL_GetTicks() - startTime;
	state = ANI_PAUSE;
}

void ELerpAnimation::resume()
{
	startTime = SDL_GetTicks() - elapsedTime;
	state = ANI_START;
}

void ELerpAnimation::setStartPosition(int x, int y)
{
	start_x = x;
	start_y = y;
}

void ELerpAnimation::setEndPosition(int x, int y)
{
	end_x = x;
	end_y = y;
}

void ELerpAnimation::setTransition(int seconds)
{
	transition_sec = seconds;
}

#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))

void ELerpAnimation::update(Uint32 currentTime, Uint32 accumulator)
{
	Uint32 compensatedTime = currentTime + accumulator;

	/* Bug fix for iOS */
	Uint32 atomicTime = (prevTime==0) ? 0 : (ß
		(compensatedTime > prevTime) ? (compensatedTime - prevTime) : (prevTime - compensatedTime));
	Uint32 elapsed = compensatedTime - startTime;
	bool checkFinished = false;
	Uint32 v;

	if (ANI_START != state)	return;

	prevTime = compensatedTime;

	//a_x = ((float)start_x + ((float)(end_x - start_x) * ((float)elapsed / 1000.0f)));
	//a_y = ((float)start_y + ((float)(end_y - start_y) * ((float)elapsed / 1000.0f)));

    v = SMOOTHSTEP(elapsed / 100000.0f);
    a_x = (start_x * v) + (end_x * (1 - v));
    a_y = (start_y * v) + (end_y * (1 - v));

	LOG_DBG("Time: %d / Elapsed: %d / [%d, %d]",
			compensatedTime, elapsed, (int)a_x, (int)a_y);

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

	if (checkFinished) {
		LOG_DBG("Finished");
		EAnimation::stop();
		a_x = 0;
		a_y = 0;
		startTime = 0;
		state = ANI_STOP;
	}
}