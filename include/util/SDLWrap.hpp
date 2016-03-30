#pragma once

#include <string>
#include <SDL.h>

class SDL_Surface_Wrap
{
public:
	SDL_Surface_Wrap(const SDL_Surface_Wrap&);
	SDL_Surface_Wrap(SDL_Surface *);
	SDL_Surface_Wrap(std::string path);
	virtual ~SDL_Surface_Wrap();
	SDL_Surface_Wrap& operator=(SDL_Surface_Wrap&& other);

	SDL_Surface* getSurface();

protected:
	SDL_Surface* surface;
};

class SDL_Texture_Wrap
{
public:
	SDL_Texture_Wrap(SDL_Texture *);
	SDL_Texture_Wrap(SDL_Surface *);
	SDL_Texture_Wrap(std::string path);
	virtual ~SDL_Texture_Wrap();

	SDL_Texture_Wrap(SDL_Texture_Wrap&) = delete;
	SDL_Texture_Wrap& operator=(SDL_Texture_Wrap&& other) = delete;

	SDL_Texture_Wrap(const SDL_Texture_Wrap& other) = delete;
	SDL_Texture_Wrap& operator=(const SDL_Texture_Wrap& other) = delete;
	SDL_Texture_Wrap(SDL_Texture_Wrap&& t) = delete;
	SDL_Texture_Wrap operator=(SDL_Texture_Wrap** t) = delete;

	SDL_Texture* getTexture();

protected:
	SDL_Texture* texture;

	bool createFromSurface(SDL_Surface *);
};
