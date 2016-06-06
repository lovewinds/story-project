#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>

class Ecore;

//Texture wrapper class
class ETexture
{
public:
	// Initializes variables
	ETexture(Ecore* core);

	// Deallocates memory
	~ETexture();

	// Loads image at specified path
	bool loadFromFile(std::string path);

	// Deallocates texture
	void free();

	// Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	// Set blending
	void setBlendMode(SDL_BlendMode blending);

	// Set alpha modulation
	void setAlpha(Uint8 alpha);

	// Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Color bgColor);

	// Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void render_resize(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void calculate(Uint32 currentTime);
	void draw();

	// Gets image dimensions
	int getWidth();
	int getHeight();

private:
	Ecore* core;

	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;

	double m_x;
	double m_y;

	double force_x;
	double force_y;

	//Angle of rotation
	double m_degrees;
};
