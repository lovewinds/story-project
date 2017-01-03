#pragma once

#include <string>
#include <vector>
#include <SDL.h>

class SDLSurfaceWrap
{
public:
	SDLSurfaceWrap(const SDLSurfaceWrap&);
	SDLSurfaceWrap(SDL_Surface *);
	SDLSurfaceWrap(std::string path);
	SDLSurfaceWrap(std::string text, SDL_Color textColor, SDL_Color bgColor);
	SDLSurfaceWrap(std::vector<std::vector<short> > gridMap);
	virtual ~SDLSurfaceWrap();
	SDLSurfaceWrap& operator=(SDLSurfaceWrap&& other);

	SDL_Surface* getSurface();

protected:
	SDL_Surface* surface;
};

class SDLTextureWrap
{
public:
	SDLTextureWrap(SDL_Texture *);
	SDLTextureWrap(SDL_Surface *);
	SDLTextureWrap(std::string path);
	SDLTextureWrap(std::string text, SDL_Color textColor, SDL_Color bgColor);
	SDLTextureWrap(std::vector<std::vector<short> > gridMap);
	virtual ~SDLTextureWrap();

	SDLTextureWrap(SDLTextureWrap&) = delete;
	/*SDLTextureWrap(const SDLTextureWrap& other) = delete;*/
	SDLTextureWrap(SDLTextureWrap&& t) = delete;

	SDLTextureWrap operator=(SDLTextureWrap** t) = delete;
	SDLTextureWrap& operator=(SDLTextureWrap&& other) = delete;
	SDLTextureWrap& operator=(const SDLTextureWrap& other) = delete;

	SDL_Texture* getTexture();
	int getWidth();
	int getHeight();

protected:
	SDL_Texture* texture;
	int width;
	int height;

	bool createFromSurface(SDL_Surface *);
};
