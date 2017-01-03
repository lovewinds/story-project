#include <SDL.h>

#include "Ecore.hpp"
#include "util/SDLWrap.hpp"
#include "util/LogHelper.hpp"


SDLSurfaceWrap::SDLSurfaceWrap(const SDLSurfaceWrap& other)
: surface(nullptr)
{
	SDLSurfaceWrap& managed = const_cast<SDLSurfaceWrap&>(other);
	surface = managed.surface;
	managed.surface = nullptr;
}

SDLSurfaceWrap::SDLSurfaceWrap(SDL_Surface *_surf)
: surface(nullptr)
{
	if (_surf)
		surface = _surf;
}

SDLSurfaceWrap::SDLSurfaceWrap(std::string path)
: surface(nullptr)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	if (path.empty()) {
		LOG_ERR("Invalid image path !");
		return;
	}

	/* Load image at specified path */
	surface = IMG_Load(path.c_str());
	if (surface == nullptr)
	{
		LOG_INFO("Unable to load image %s! SDL_image Error: %s\n",
				path.c_str(), IMG_GetError());
		return;
	}
	/* Color key image */
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));
}

SDLSurfaceWrap::SDLSurfaceWrap(std::string text, SDL_Color textColor, SDL_Color bgColor)
: surface(nullptr)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();
	TTF_Font* gFont = Ecore::getInstance()->getFont();

	if (text.empty()) {
		LOG_ERR("Text for texture is empty !");
		return;
	}

	/* Render text surface */
	//surface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
	//surface = TTF_RenderText_Shaded(gFont, text.c_str(), textColor, bgColor);
	//surface = TTF_RenderText_Blended(gFont, text.c_str(), textColor);
	surface = TTF_RenderUTF8_Blended(gFont, text.c_str(), textColor);
	if (surface == nullptr)
	{
		LOG_ERR("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return;
	}
}

SDLSurfaceWrap::SDLSurfaceWrap(std::vector<std::vector<short> > map)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();
	Uint32 rmask, gmask, bmask, amask;
	/* TODO: Below size should be taken from arg */
	const int TILE_SIZE = 32;
	int width = 8 * TILE_SIZE;
	int height = 10 * TILE_SIZE;

	/* Use default masks */
	surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	if (nullptr == surface)
	{
		LOG_ERR("Unable to create GridMap surface!");
		return;
	}
}

SDLSurfaceWrap::~SDLSurfaceWrap()
{
	if (surface) {
		/* Deallocate loaded surface */
		SDL_FreeSurface(surface);
		surface = nullptr;
	}
}

SDLSurfaceWrap& SDLSurfaceWrap::operator=(SDLSurfaceWrap&& other)
{
	if (&other == this)
		return *this;
	if (surface != nullptr)
		SDL_FreeSurface(surface);
	surface = other.surface;
	other.surface = nullptr;
	return *this;
}

SDL_Surface* SDLSurfaceWrap::getSurface()
{
	return surface;
}



///////////////////////////////////////////////////////////////////////////////
//
// SDLTextureWrap
//
///////////////////////////////////////////////////////////////////////////////
bool SDLTextureWrap::createFromSurface(SDL_Surface *surface)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	if (!surface)
		return false;

	width = surface->w;
	height = surface->h;

	/* Create texture from surface pixels */
	texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	if (texture == nullptr) {
		LOG_INFO("Unable to create texture from Surface[%p]! SDL Error: %s\n",
				surface, SDL_GetError());
	}
	LOG_DBG("    SDL_Texture created [%p]", texture);
	return true;
}

SDLTextureWrap::SDLTextureWrap(SDL_Texture *_texture)
: texture(nullptr)
{
	if (_texture)
		texture = _texture;
	LOG_DBG("    SDL_Texture created");
}

SDLTextureWrap::SDLTextureWrap(SDL_Surface *surface)
: texture(nullptr)
{
	createFromSurface(surface);
}

SDLTextureWrap::SDLTextureWrap(std::string path)
: texture(nullptr)
{
	std::shared_ptr<SDLSurfaceWrap> surf(new SDLSurfaceWrap(path));

	if (surf) {
		createFromSurface(surf->getSurface());
	} else {
		LOG_ERR("Failed to create surface !");
	}
}

SDLTextureWrap::SDLTextureWrap(std::string text, SDL_Color textColor, SDL_Color bgColor)
: texture(nullptr)
{
	std::shared_ptr<SDLSurfaceWrap> surf(new SDLSurfaceWrap(text, textColor, bgColor));

	if (surf) {
		createFromSurface(surf->getSurface());
	} else {
		LOG_ERR("Failed to create surface !");
	}
}

SDLTextureWrap::SDLTextureWrap(std::vector<std::vector<short> > map)
: texture(nullptr)
{
	std::shared_ptr<SDLSurfaceWrap> surf(new SDLSurfaceWrap(map));

	if (surf) {
		if (createFromSurface(surf->getSurface()))
		{
			/* Set rendering target */
			SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();
			SDL_SetRenderTarget(gRenderer, texture);

			

			/* Reset rendering target */
			SDL_SetRenderTarget(gRenderer, NULL);
		}
	} else {
		LOG_ERR("Failed to create surface !");
	}
}
#if 0
SDLTextureWrap::SDLTextureWrap(SDLTextureWrap&& t) : texture(t.texture)
{
	LOG_ERR("Move constructor");
	t.texture = nullptr;
}

SDLTextureWrap::SDLTextureWrap(SDLTextureWrap& other)
{
	LOG_ERR("const Copy constructor");
	texture = other.texture;
	other.texture = nullptr;
}

SDLTextureWrap& SDLTextureWrap::operator=(SDLTextureWrap&& other)
{
	LOG_ERR("allocate constructor");
	if (&other == this)
		return *this;
	if (texture != nullptr)
		SDL_DestroyTexture(texture);
	texture = other.texture;
	other.texture = nullptr;
	return *this;
}
#endif
SDLTextureWrap::~SDLTextureWrap()
{
	if (texture != nullptr) {
		/* Deallocate texture */
		LOG_DBG("    SDL_Texture removed [%d x %d] [%p]", width, height, texture);
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

SDL_Texture* SDLTextureWrap::getTexture()
{
	return texture;
}

int SDLTextureWrap::getWidth()
{
	return width;
}

int SDLTextureWrap::getHeight()
{
	return height;
}
