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

/**
 * EGridDesc class
 */
EGridDesc::EGridDesc(int width, int height)
{
	this->width = width;
	this->height = height;

	gridArray = new short **[width];
	for(int col = 0; col < width; col++) {
		gridArray[col] = new short *[height];
		for(int row = 0; row < height; row++) {
			gridArray[col][row] = new short [GRID_LAYER_END];
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

void EGridDesc::setGridValue(EGridLayerID layerId, int x, int y, short value)
{
	if (0 <= x && x < width) {
		if (0 <= y && y < height) {
			gridArray[x][y][layerId] = value;
		}
	}
}

short EGridDesc::getGridValue(EGridLayerID layerId, int x, int y)
{
	short res = -1;

	if (0 <= x && x < width) {
		if (0 <= y && y < height) {
			res = gridArray[x][y][layerId];
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
