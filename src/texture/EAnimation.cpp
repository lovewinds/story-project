#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "texture/GraphicObject.hpp"

#include "texture/EAnimation.hpp"

EAnimation::EAnimation() :
a_x(0), a_y(0), a_angle(0)
{
	startTime = 0;
    elapsedTime = 0;
    state = ANI_STOP;
}

EAnimation::~EAnimation()
{
	LOG_ERR("Removed animation");
}

double EAnimation::getX()
{
    return a_x;
}

double EAnimation::getY()
{
    return a_y;
}

double EAnimation::getAngle()
{
    return a_angle;
}

void EAnimation::start()
{
    startTime = Ecore::getAppTicks();
    state = ANI_START;
}

void EAnimation::stop()
{
    startTime = 0;
    state = ANI_STOP;

	std::shared_ptr<story::Graphic::Object> t = caller.lock();
	if (t)
		t->finishedAnimationCallback(a_x, a_y);
}

void EAnimation::sync()
{
	std::shared_ptr<story::Graphic::Object> t = caller.lock();
	if (t)
		t->syncAnimationCallback(a_x, a_y);
}


void EAnimation::pause()
{
    elapsedTime = Ecore::getAppTicks() - startTime;
    state = ANI_PAUSE;
}

void EAnimation::resume()
{
    startTime = Ecore::getAppTicks() - elapsedTime;
    state = ANI_START;
}

AnimationState EAnimation::getState()
{
	return state;
}

void EAnimation::setCaller(std::weak_ptr<story::Graphic::Object> clr)
{
	/* Store weak_ptr to avoid circular reference */
	caller = clr;
}

void EAnimation::update(Uint32 currentTime, Uint32 accumulator)
{
    static double dir = 1.0f;
	static unsigned int count = 0;
	static double accel = 0.026;
	static double velo = 1.0;

	static Uint32 prevTime = 0;
	Uint32 compensatedTime = currentTime;
	Uint32 atomicTime = (compensatedTime - startTime);
	Uint32 delta = atomicTime - prevTime;
	static double prev_y = 0;

	if (atomicTime == 0) {
		velo = 1.0;
		accel = 0.0;
	}

	if (ANI_START != state)	return;
/*
	if (atomicTime > 1000) {
		animating = false;
	}
*/
	accel = 0.0;
	velo = 0.1;

	//prev_y = a_y + velo * ((atomicTime > 375) ? 375 : atomicTime);
	a_y = velo * ((atomicTime > 375) ? 375 : atomicTime);

	if (atomicTime > 375) {
		delta = atomicTime - 375;
		accel = -0.00026;
		//velo = 1.0 - (1.0 / 375.0) * (delta - 375);
		velo += accel * delta;
		a_y += velo * delta;
	}

	/* Recover */
	if (a_y < -0.1) {
		a_y = 0.0;
		startTime = currentTime;
		accel = 0.0;
		velo = 0.1;
	}

	a_x = a_y;
	prevTime = delta;
}