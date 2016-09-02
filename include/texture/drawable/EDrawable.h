#pragma once
#include "texture/ETexture.hpp"

class EDrawable : public ETexture
{
public:
	EDrawable();
	EDrawable(int x, int y);

	virtual ~EDrawable();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

	/* Animation */
	void animateStart(Uint32 startTime);

protected:
	/* animation */
	bool animating;
	Uint32 startTime;

	/* drawing */
	int radian;
	int alpha;
};
