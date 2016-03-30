#include "Ecore.hpp"
#include "resource/ESpriteInfo.hpp"
#include "resource/EResourceManager.hpp"

#include "util/LogHelper.hpp"

ESpriteInfo::ESpriteInfo(std::string name, std::string image_res)
{
	LOG_INFO("ESpriteInfo[%s] created", name.c_str());

	EResourceManager &resManager = Ecore::getInstance()->getResourceManager();
	std::shared_ptr<EImageResourceInfo> imgRes = resManager.getImageResource(image_res);

	if (imgRes) {
		this->texture = imgRes->getTexture();
		LOG_INFO("Sprite refers texture [0x%p]", this->texture->getTexture());
	} else {
		LOG_ERR("Cannot found Image resource [%s] from map !", image_res.c_str());
	}
	this->name = name;
}

ESpriteInfo::~ESpriteInfo()
{
	LOG_INFO("ESpriteInfo[%s] removed", name.c_str());
}

std::string ESpriteInfo::getName()
{
	return name;
}

void ESpriteInfo::setSpriteCells()
{

}

bool ESpriteInfo::allocateSprite()
{
	return true;
}
