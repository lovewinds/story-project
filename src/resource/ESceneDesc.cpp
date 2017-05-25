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
	controllable = false;
}

std::string ESpriteDesc::getName()
{
	return name;
}

std::string ESpriteDesc::getType()
{
	return type;
}

int ESpriteDesc::getX()
{
	return pos_x;
}

int ESpriteDesc::getY()
{
	return pos_y;
}

void ESpriteDesc::setControllable(bool controllable)
{
	this->controllable = controllable;
}

bool ESpriteDesc::isControllable()
{
	return controllable;
}

EImageDesc::EImageDesc(std::string name, std::string sourceType, int x, int y)
{
	this->name = name;
	this->type = sourceType;
	this->pos_x = x;
	this->pos_y = y;
	this->h_align = IMAGE_ALIGN_LEFT;
	this->v_align = IMAGE_ALIGN_TOP;
}

void EImageDesc::setWidth(double width)
{
	this->width = width;
}

void EImageDesc::setHeight(double height)
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

void EImageDesc::setHorizontalAlign(EImageHorizontalAlign h_align)
{
	this->h_align = h_align;
}

void EImageDesc::setVerticalAlign(EImageVerticalAlign v_align)
{
	this->v_align = v_align;
}

std::string EImageDesc::getName()
{
	return name;
}

std::string EImageDesc::getType()
{
	return type;
}

int EImageDesc::getX()
{
	return pos_x;
}

int EImageDesc::getY()
{
	return pos_y;
}

double EImageDesc::getWidth()
{
	return width;
}

double EImageDesc::getHeight()
{
	return height;
}

double EImageDesc::getWidthRatio()
{
	return width_ratio;
}

double EImageDesc::getHeightRatio()
{
	return height_ratio;
}

EImageHorizontalAlign EImageDesc::getHorizontalAlign()
{
	return h_align;
}

EImageVerticalAlign EImageDesc::getVerticalAlign()
{
	return v_align;
}

/**
 * EGridDesc class
 */
EGridDesc::EGridDesc(int width, int height, int levels)
{
	this->width = width;
	this->height = height;
	this->levels = levels;

	gridArray = new unsigned short **[width];
	for(int col = 0; col < width; col++) {
		gridArray[col] = new unsigned short *[height];
		for(int row = 0; row < height; row++) {
			gridArray[col][row] = new unsigned short[levels];
			for (int ll = 0; ll < levels; ll++) {
				gridArray[col][row][ll] = 9999;
			}
		}
	}
}

EGridDesc::~EGridDesc()
{
	for(int col = 0; col < width; col++) {
		for(int row = 0; row < GRID_LAYER_END; row++) {
			delete[] gridArray[col][row];
		}
		delete[] gridArray[col];
	}
	delete[] gridArray;
}

void EGridDesc::setGridValue(int level, int x, int y, unsigned short value)
{
	if (0 <= x && x < width) {
		if (0 <= y && y < height) {
			gridArray[x][y][level] = value;
		}
	}
}

unsigned short EGridDesc::getGridValue(int level, int x, int y)
{
	unsigned short res = 9999;

	if (0 <= x && x < width) {
		if (0 <= y && y < height) {
			res = gridArray[x][y][level];
		}
	}

	return res;
}

int EGridDesc::getGridWidth()
{
	return width;
}

int EGridDesc::getGridHeight()
{
	return height;
}

int EGridDesc::getGridLevels()
{
	return levels;
}

/**
 * ESceneLayerDesc class
 */

ESceneLayerDesc::ESceneLayerDesc(std::string name) :
	sprite_list(_sprite_list),
	image_list(_image_list),
	grid_list(_grid_list)
{
	this->name = name;
}

void ESceneLayerDesc::addSpriteDesc(std::shared_ptr<ESpriteDesc> desc)
{
	if (nullptr != desc)
		_sprite_list.push_back(desc);
}

void ESceneLayerDesc::addImageDesc(std::shared_ptr<EImageDesc> desc)
{
	if (nullptr != desc)
		_image_list.push_back(desc);
}

void ESceneLayerDesc::addGridDesc(std::shared_ptr<EGridDesc> desc)
{
	if (nullptr != desc)
		_grid_list.push_back(desc);
}

std::string ESceneLayerDesc::getName()
{
	return name;
}

/**
 * ESceneDesc class
 */

ESceneDesc::ESceneDesc(std::string name, ESceneType type)
: layer_list(_layer_list)
{
	this->name = name;
	this->type = type;
}

void ESceneDesc::appendLayerDesc(std::shared_ptr<ESceneLayerDesc> layer)
{
	if (nullptr != layer)
		_layer_list.push_back(layer);
}

std::string ESceneDesc::getName()
{
	return name;
}

ESceneType ESceneDesc::getType()
{
	return type;
}
