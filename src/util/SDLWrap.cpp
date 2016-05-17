#include <SDL.h>

#include "Ecore.hpp"
#include "util/SDLWrap.hpp"
#include "util/LogHelper.hpp"


SDL_Surface_Wrap::SDL_Surface_Wrap(const SDL_Surface_Wrap& other)
: surface(nullptr)
{
	SDL_Surface_Wrap& managed = const_cast<SDL_Surface_Wrap&>(other);
	surface = managed.surface;
	managed.surface = nullptr;
}

SDL_Surface_Wrap::SDL_Surface_Wrap(SDL_Surface *_surf)
: surface(nullptr)
{
	if (_surf)
		surface = _surf;
}

SDL_Surface_Wrap::SDL_Surface_Wrap(std::string path)
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

SDL_Surface_Wrap::~SDL_Surface_Wrap()
{
	if (surface) {
		/* Deallocate loaded surface */
		SDL_FreeSurface(surface);
		surface = nullptr;
	}
}

SDL_Surface_Wrap& SDL_Surface_Wrap::operator=(SDL_Surface_Wrap&& other)
{
	if (&other == this)
		return *this;
	if (surface != nullptr)
		SDL_FreeSurface(surface);
	surface = other.surface;
	other.surface = nullptr;
	return *this;
}

SDL_Surface* SDL_Surface_Wrap::getSurface()
{
	return surface;
}



///////////////////////////////////////////////////////////////////////////////
//
// SDL_Texture_Wrap
//
///////////////////////////////////////////////////////////////////////////////
bool SDL_Texture_Wrap::createFromSurface(SDL_Surface *surface)
{
	SDL_Renderer *gRenderer = Ecore::getInstance()->getRenderer();

	if (!surface)
		return false;

	/* Create texture from surface pixels */
	texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	if (texture == nullptr) {
		LOG_INFO("Unable to create texture from Surface[%p]! SDL Error: %s\n",
				surface, SDL_GetError());
	}
	LOG_DBG("    SDL_Texture created [%p]", texture);
	return true;
}

SDL_Texture_Wrap::SDL_Texture_Wrap(SDL_Texture *_texture)
: texture(nullptr)
{
	if (_texture)
		texture = _texture;
	LOG_DBG("    SDL_Texture created");
}

SDL_Texture_Wrap::SDL_Texture_Wrap(SDL_Surface *surface)
: texture(nullptr)
{
	createFromSurface(surface);
}

SDL_Texture_Wrap::SDL_Texture_Wrap(std::string path)
: texture(nullptr)
{
	std::shared_ptr<SDL_Surface_Wrap> surf(new SDL_Surface_Wrap(path));

	if (surf) {
		createFromSurface(surf->getSurface());
	} else {
		LOG_ERR("Failed to create surface !");
	}
}
#if 0
SDL_Texture_Wrap::SDL_Texture_Wrap(SDL_Texture_Wrap&& t) : texture(t.texture)
{
	LOG_ERR("Move constructor");
	t.texture = nullptr;
}

SDL_Texture_Wrap::SDL_Texture_Wrap(SDL_Texture_Wrap& other)
{
	LOG_ERR("const Copy constructor");
	texture = other.texture;
	other.texture = nullptr;
}

SDL_Texture_Wrap& SDL_Texture_Wrap::operator=(SDL_Texture_Wrap&& other)
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
SDL_Texture_Wrap::~SDL_Texture_Wrap()
{
	if (texture != nullptr) {
		/* Deallocate texture */
		LOG_DBG("    SDL_Texture removed [%p]", texture);
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

SDL_Texture* SDL_Texture_Wrap::getTexture()
{
	return texture;
}
