#pragma once
#include "EDrawable.hpp"

class EFigure : public EDrawable
{
public:
	EFigure();
	EFigure(int x, int y);

	virtual ~EFigure();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

	/* Animation */
	virtual void startAnimation();
	virtual void stopAnimation();

protected:
	/* animation */
	bool animating;
	Uint32 startTime;

	/* drawing */
	int radian;
	int alpha;
};
