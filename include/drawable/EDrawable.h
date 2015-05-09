#pragma once
#include <SDL.h>

class EDrawable
{
public:
	EDrawable();
	EDrawable(int x, int y);

	virtual ~EDrawable();

	/* Set blending */
	void setBlendMode(SDL_BlendMode blending);

	/* Set alpha modulation */
	void setAlpha(Uint8 alpha);

	/* Set color modulation */
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void draw(Uint32 currentTime, Uint32 accumulator = 0);

	/* Animation */
	void animateStart(Uint32 startTime);

protected:
	/* The actual SDL texture */
	SDL_Texture* mTexture;

	/* animation */
	bool animating;
	Uint32 startTime;

	/* Position */
	double m_x;
	double m_y;

	double p_x;
	double p_y;

	/* drawing */
	int radian;
	int alpha;
};
