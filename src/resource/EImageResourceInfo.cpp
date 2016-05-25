#include <SDL_image.h>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResourceInfo.hpp"

EImageResourceInfo::EImageResourceInfo(std::string name, std::string path, unsigned int width, unsigned int height)
{
	this->name = name;
	this->path = Ecore::getResourcePath(path);
	this->width = width;
	this->height = height;

	LOG_INFO("EImageResourceInfo[%s] created", this->name.c_str());
	LOG_INFO("      path : [%s]", this->path.c_str());

	/* TODO: Don't allocate on creation */
	//allocate();
}

EImageResourceInfo::~EImageResourceInfo()
{
	releaseTexture();
	LOG_INFO("EImageResourceInfo[%s] removed", name.c_str());
}

std::string EImageResourceInfo::getName() const
{
	return name;
}

std::string EImageResourceInfo::getPath() const
{
	return path;
}

unsigned int EImageResourceInfo::getWidth() const
{
	return width;
}

unsigned int EImageResourceInfo::getHeight() const
{
	return height;
}

std::shared_ptr<SDL_Texture_Wrap> EImageResourceInfo::getTexture()
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

void EImageResourceInfo::releaseTexture()
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
void EImageResourceInfo::allocate()
{
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();

	texture = resManager.allocateTexture(path);
	if (!texture) {
		LOG_ERR("Failed to allocate resource [%s]", getPath().c_str());
	}

	return;
}
#endif
