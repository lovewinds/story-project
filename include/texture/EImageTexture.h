#pragma once
#include <stdio.h>
#include <string>
#include <cmath>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "ETexture.h"

class EImageTexture : public ETexture
{
public:
	/* Initializes variables */
	EImageTexture();
	EImageTexture(int x, int y);

	/* Deallocates memory */
	~EImageTexture();

	/* Loads image at specified path */
	bool loadFromFile(std::string path);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	//virtual void paint(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

	/* Gets image dimensions */
	int getWidth();
	int getHeight();

protected:
	int radian;

	/* Angle of rotation */
	double m_degrees;

	/* animation */
	bool animating;
};
