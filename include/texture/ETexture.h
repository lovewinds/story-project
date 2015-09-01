#pragma once
#include <SDL.h>

class ETexture
{
public:
	ETexture();
	virtual ~ETexture();

	/* Deallocates texture */
	virtual void free();

	/* Set blending */
	void setBlendMode(SDL_BlendMode blending);

	/* Set alpha modulation */
	void setAlpha(Uint8 alpha);

	/* Set color modulation */
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	/* Position */
	void movePositionTo(double x, double y);
	void movePositionBy(double delta_x, double delta_y);

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0) = 0;
	virtual void render() = 0;

protected:
	/* The actual SDL texture */
	SDL_Texture* mTexture;

	/* Position */
	double p_x;
	double p_y;

	/* Texture dimensions */
	int mWidth;
	int mHeight;

	/* Renders texture at given point */
	virtual void texture_render(int x, int y,
		SDL_Rect* clip = NULL,
		double angle = 0.0,
		SDL_Point* center = NULL,
		SDL_RendererFlip flip = SDL_FLIP_NONE);

	virtual void texture_render_resize(int x, int y,
		SDL_Rect* clip = NULL,
		Uint8 expand = 1,
		double angle = 0.0,
		SDL_Point* center = NULL,
		SDL_RendererFlip flip = SDL_FLIP_NONE);
};
