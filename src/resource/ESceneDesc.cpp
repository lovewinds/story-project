#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "texture/ESprite.hpp"

#include "resource/ESceneDesc.hpp"

ESpriteDesc::ESpriteDesc(std::string name, std::string sourceType, int x, int y)
{
	this->name = name;
	this->type = sourceType;
	this->pos_x = x;
	this->pos_y = y;
}

EImageDesc::EImageDesc(std::string name, std::string sourceType, int x, int y)
{
	this->name = name;
	this->type = sourceType;
	this->pos_x = x;
	this->pos_y = y;
}

void EImageDesc::setWidth(int width)
{
	this->width = width;
}

void EImageDesc::setHeight(int height)
{
	this->height = height;
}

void EImageDesc::setWidthRatio(double width)
{
	this->width_ratio = width;
}

void EImageDesc::setHeightRatio(double height)
{
	this->height_ratio = height;
}

/**
 * ESceneLayerDesc class
 */

ESceneLayerDesc::ESceneLayerDesc(std::string name)
{
	this->name = name;
}

bool ESceneLayerDesc::addSpriteDesc(std::string name, std::string sourceResource,
		int x, int y)
{
	std::shared_ptr<ESpriteDesc> desc
		 = std::shared_ptr<ESpriteDesc>(new ESpriteDesc(name, sourceResource, x, y));

	sprite_list.push_back(desc);
}

bool ESceneLayerDesc::addImageDesc(std::string name, std::string sourceResource,
			int x, int y,
			int width, int height,
			int width_percent, int height_percent)
{
	std::shared_ptr<ESpriteDesc> desc
		 = std::shared_ptr<ESpriteDesc>(new ESpriteDesc(name, sourceResource, x, y));

	sprite_list.push_back(desc);
}
