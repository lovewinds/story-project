#pragma once

#include <SDL.h>

/* Avoid circular reference */
class ETexture;

enum AnimationState
{
	ANI_NONE,
	ANI_STOP,
	ANI_START,
	ANI_PAUSE,
	ANI_RESUME,
};

class EAnimation
{
public:
	EAnimation();
	virtual ~EAnimation();

	virtual void start();
	virtual void stop();
    virtual void pause();
    virtual void resume();
	virtual void sync();

    double getX();
    double getY();
	AnimationState getState();

	void setCaller(std::shared_ptr<ETexture> caller);

    virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
    Uint32 startTime;
    Uint32 elapsedTime;
    AnimationState state;

	/* This model only supports one callback at one time. */
	/* TODO: Use callback list */
	//std::weak_ptr<ETexture> caller;

	std::weak_ptr<ETexture> caller;

	/* Position */
	double a_x;
	double a_y;
};
