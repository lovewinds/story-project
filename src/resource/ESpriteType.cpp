#include "core/Ecore.hpp"
#include "resource/EResourceManager.hpp"
#include "util/LogHelper.hpp"

#include "resource/ESpriteType.hpp"

namespace story {
namespace Resource {

ESpriteType::ESpriteType(std::string name, std::string base_image)
: cells(_cell)
{
	LOG_INFO("ESpriteType[%s] created", name.c_str());

	this->name = name;
	this->base_image = base_image;
	this->cell_width = 0;
	this->cell_height = 0;
	this->base_width = 0;
	this->base_height = 0;
}

ESpriteType::~ESpriteType()
{
	/* TODO: SpriteInfo should notice its release to resource manager */
	//EResourceManager &resManager = story::Core::Ecore::getInstance()->getResourceManager();
	//resManager.releaseTexture();

	LOG_INFO("ESpriteType[%s] removed", name.c_str());
}

std::string ESpriteType::getName()
{
	return name;
}

std::string ESpriteType::getBaseImage()
{
	return base_image;
}

bool ESpriteType::setCellInfo(unsigned int cell_width, unsigned int cell_height)
{
	EResourceManager &resManager = story::Core::Ecore::getInstance()->getResourceManager();
	std::shared_ptr<EImageResource> res = resManager.getImageResource(base_image);
	if (res) {
		this->base_width = res->getWidth();
		this->base_height = res->getHeight();
	} else {
		LOG_ERR("Cannot find base image resource [%s]", base_image.c_str());
		return false;
	}

	this->cell_width = cell_width;
	this->cell_height = cell_height;

	LOG_DBG("   SpriteType Name: [%s] / Src: [%s] / Cell: (%d x %d) / Base: (%d x %d)",
			name.c_str(), base_image.c_str(), cell_width, cell_height, base_width, base_height);
	return true;
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
	rect.y = cell_height * y;
	rect.w = cell_width;
	rect.h = cell_height;
	LOG_DBG("Cell [%d] (%d,%d) [%d/%d]", index, rect.x, rect.y, rect.w, rect.h);

	_cell.push_back(rect);

	return true;
}

} /* namespace Resource */
} /* namespace story */
