#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

class SDLFontWrap
{
public:
	SDLFontWrap(TTF_Font *, std::string family, int size);
	virtual ~SDLFontWrap();

	SDLFontWrap(SDLFontWrap&) = delete;
	SDLFontWrap(SDLFontWrap&& t) = delete;

	SDLFontWrap operator=(SDLFontWrap** t) = delete;
	SDLFontWrap& operator=(SDLFontWrap&& other) = delete;
	SDLFontWrap& operator=(const SDLFontWrap& other) = delete;

	TTF_Font* getFont();
	std::string getFamily();
	int getSize();

protected:
	TTF_Font* font;
	std::string family;
	int size;
};

class SDLSurfaceWrap
{
public:
	SDLSurfaceWrap(const SDLSurfaceWrap&);
	SDLSurfaceWrap(SDL_Surface *);
	SDLSurfaceWrap(std::string path);
	SDLSurfaceWrap(std::string text,
			SDL_Color textColor, SDL_Color bgColor,
			std::shared_ptr<SDLFontWrap> font);
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
	SDLTextureWrap(std::string text,
			SDL_Color textColor, SDL_Color bgColor,
			std::shared_ptr<SDLFontWrap> font);
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
