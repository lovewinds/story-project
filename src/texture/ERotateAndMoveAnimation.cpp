#include "Ecore.hpp"
#include "util/LogHelper.hpp"

#include "texture/ERotateAndMoveAnimation.hpp"

ERotateAndMoveAnimation::ERotateAndMoveAnimation()
{
}

ERotateAndMoveAnimation::~ERotateAndMoveAnimation()
{
}

void ERotateAndMoveAnimation::start()
{
	prevTime = SDL_GetTicks();

	EAnimation::start();
}

void ERotateAndMoveAnimation::stop()
{
	EAnimation::stop();
}

void ERotateAndMoveAnimation::pause()
{
	elapsedTime = SDL_GetTicks() - startTime;
	state = ANI_PAUSE;
}

void ERotateAndMoveAnimation::resume()
{
	startTime = SDL_GetTicks() - elapsedTime;
	state = ANI_START;
}

void ERotateAndMoveAnimation::setStartPosition(int x, int y)
{
	start_x = x;
	start_y = y;
}

void ERotateAndMoveAnimation::setEndPosition(int x, int y)
{
	end_x = x;
	end_y = y;
}

void ERotateAndMoveAnimation::setRotateDirection(RotateDirection dir)
{
	direction = dir;
}

void ERotateAndMoveAnimation::setTransition(int milliseconds)
{
	transition_msec = milliseconds;
}

#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))

void ERotateAndMoveAnimation::update(Uint32 currentTime, Uint32 accumulator)
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

	dt = ((float)elapsed) / (float)(transition_msec);
	if (dt >= 1.0f)
		checkFinished = true;

	v = SMOOTHSTEP(dt);
	/* Angle */
	if (direction == ROTATE_CLOCKWISE)
		a_angle = (720.0f * v) + (0.0f * (1 - v));
	else
		a_angle = (0.0f * v) + (720.0f * (1 - v));
	/* Position */
	/* NON-smooth */
	a_x = (end_x * dt) + (start_x * (1 - dt));
    a_y = (end_y * dt) + (start_y * (1 - dt));

	if (checkFinished) {
		LOG_DBG("Finished");
		EAnimation::stop();
		a_angle = 0.0f;
		startTime = 0;
		state = ANI_STOP;
	}
}