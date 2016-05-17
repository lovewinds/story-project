#include "Ecore.hpp"
#include "resource/EResourceManager.hpp"
#include "util/LogHelper.hpp"

#include "resource/ESpriteType.hpp"

ESpriteType::ESpriteType(std::string name, std::string base_image)
{
	LOG_INFO("ESpriteType[%s] created", name.c_str());

	this->name = name;
	this->base_image = base_image;
}

ESpriteType::~ESpriteType()
{
	/* TODO: SpriteInfo should notice its release to resource manager */
	//EResourceManager &resManager = Ecore::getInstance()->getResourceManager();
	//resManager.releaseTexture();

	LOG_INFO("ESpriteType[%s] removed", name.c_str());
}

std::string ESpriteType::getName()
{
	return name;
}

void ESpriteType::setBaseImageInfo(unsigned int image_width, unsigned int image_height)
{
	this->base_width = image_width;
	this->base_height = image_height;
}

void ESpriteType::setCellInfo(unsigned int cell_width, unsigned int cell_height)
{
	this->cell_width = cell_width;
	this->cell_height = cell_height;
}

/* Store sprite cell information to create */
bool ESpriteType::appendSpriteCell(unsigned int index)
{
	// wid 4
	// cel 1
	// 0 1 2 3
	// 4 5 6 7
	/* Starts from 0 */
	index -= 1;
	SDL_Rect rect;
	unsigned int row_count = (unsigned int)(base_width / cell_width);
	unsigned int col_count = (unsigned int)(base_height / cell_height);
	unsigned int x = (unsigned int)(index % row_count);
	unsigned int y = (unsigned int)(index / row_count);

	if (row_count == 0 || col_count == 0) {
		return false;
	}
	rect.x = cell_width * x;
	rect.y = cell_width * y;
	rect.w = cell_width;
	rect.h = cell_height;

	cell.push_back(rect);

	return true;
}

std::shared_ptr<ESprite> ESpriteType::createSprite(std::string spriteID, int x, int y)
{
	if (spriteID.empty() || base_image.empty() || cell.size() == 0 )
		return nullptr;

	std::shared_ptr<ESprite> sprite(new ESprite(spriteID, base_image, cell));
	if (sprite)
		sprite->movePositionTo(x, y);
	return sprite;
}
