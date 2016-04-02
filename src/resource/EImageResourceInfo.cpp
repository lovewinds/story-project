#include <SDL_image.h>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResourceInfo.hpp"

EImageResourceInfo::EImageResourceInfo(std::string name, std::string path)
{
	this->name = name;
	this->path = Ecore::getResourcePath(path);

	LOG_INFO("EImageResourceInfo[%s] created", this->name.c_str());
	LOG_INFO("      path : [%s]", this->path.c_str());

	/* Don't allocate on creation */
	allocate();
}

EImageResourceInfo::~EImageResourceInfo()
{
	/* TODO: Notify destruction to ResourceManager.
	 *       It will deallocate unused resource from memory.
	 */
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	resManager.releaseTexture(name);

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

std::shared_ptr<SDL_Texture_Wrap> EImageResourceInfo::getTexture()
{
	return texture;
}

void EImageResourceInfo::allocate()
{
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();

	texture = resManager.allocateTexture(path);
	if (!texture) {
		LOG_ERR("Failed to allocate resource [%s]", getPath().c_str());
	}

	return;
}
