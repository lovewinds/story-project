#include "Ecore.hpp"
#include "util/LogHelper.hpp"

#include "graphic/animation/ERotateAndMoveAnimation.hpp"

namespace story {
namespace Graphic {

ERotateAndMoveAnimation::ERotateAndMoveAnimation()
{
}

ERotateAndMoveAnimation::~ERotateAndMoveAnimation()
{
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
	if (!reverse) {
		a_x = (end_x * v) + (start_x * (1 - v));
	    a_y = (end_y * v) + (start_y * (1 - v));
	} else {
		a_x = (start_x * v) + (end_x * (1 - v));
	    a_y = (start_y * v) + (end_y * (1 - v));
	}

	if (checkFinished) {
		LOG_DBG("Reverse !");
		//EAnimation::stop();
		a_angle = 0.0f;
		startTime = Ecore::getAppTicks();
		//state = ANI_STOP;
		if (reverse)
			reverse = false;
		else
			reverse = true;

		if (direction == ROTATE_CLOCKWISE)
			direction = ROTATE_ANTICLOCKWISE;
		else
			direction = ROTATE_CLOCKWISE;
	}
}

} /* namespace Graphic */
} /* namespace story */
