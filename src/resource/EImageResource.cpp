#include <SDL_image.h>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResource.hpp"

EImageResource::EImageResource(std::string name, std::string path, unsigned int width, unsigned int height)
{
	this->name = name;
	this->path = Ecore::getResourcePath(path);
	this->width = width;
	this->height = height;

	LOG_INFO("EImageResource[%s] created", this->name.c_str());
	LOG_INFO("      path : [%s]", this->path.c_str());

	/* TODO: Don't allocate on creation */
	//allocate();
}

EImageResource::~EImageResource()
{
	releaseTexture();
	LOG_INFO("EImageResource[%s] removed", name.c_str());
}

std::string EImageResource::getName() const
{
	return name;
}

std::string EImageResource::getPath() const
{
	return path;
}

unsigned int EImageResource::getWidth() const
{
	return width;
}

unsigned int EImageResource::getHeight() const
{
	return height;
}

std::shared_ptr<SDLTextureWrap> EImageResource::getTexture()
{
	if (texture == nullptr) {
		EResourceManager& resManager = Ecore::getInstance()->getResourceManager();

		texture = resManager.allocateTexture(path);
		if (!texture) {
			LOG_ERR("Failed to allocate resource [%s]", getPath().c_str());
			return nullptr;
		}
	}

	return texture;
}

void EImageResource::releaseTexture()
{
	texture.reset();
	texture = nullptr;

	/* Notify destruction to ResourceManager.
	* It will deallocate unused resource from memory.
	*/
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	resManager.releaseTexture(path);
}
#if 0
void EImageResource::allocate()
{
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();

	texture = resManager.allocateTexture(path);
	if (!texture) {
		LOG_ERR("Failed to allocate resource [%s]", getPath().c_str());
	}

	return;
}
#endif
