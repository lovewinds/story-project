#pragma once
#include "ETexture.h"
#include "Ecore.h"

class EScreenManager
{
public:
	EScreenManager();
	~EScreenManager();

	/* Temporary Functions for adaptation */
	bool loadMedia();
	void start(Uint32 s);
	void render(double d_fps);
	void update(Uint32 currentTime, Uint32 accumulator);

	void handleEvent(SDL_Event e);

	ETexture boxTexture;
	ETexture textTexture;
};
