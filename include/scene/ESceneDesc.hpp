#pragma once

#include <string>
#include <map>
#include <vector>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"
#include "texture/ETextTexture.hpp"
#include "texture/EFigure.hpp"
#include "object/animation/EAnimation.hpp"

/* TODO: Inherit serializable class */

enum ESceneType
{
	SCENE_RPG = 0,
	SCENE_VISUAL_NOVEL,
	SCENE_DEBUG,
	SCENE_TITLE
};

/*
 * This class stores only resource information for actual allocation.
 * All of these ~~Desc classes are stored in ResourceManager.
 * When playing scene will be changed, SceneManager allocates actual Scene instance
 * with this SceneDesc instance on ResourceManager.
 */

class ESpriteDesc
{
public:
	ESpriteDesc(std::string name, std::string sourceType, int x, int y);

	void setControllable(bool controllable);
	bool isControllable();

	std::string getName();
	std::string getType();
	int getX();
	int getY();

protected:
	std::string name;
	std::string type;
	int pos_x;
	int pos_y;
	bool controllable;
};

/* TODO: Should be expanded to all objects */
enum EImageHorizontalAlign {
	IMAGE_ALIGN_LEFT = 0,
	IMAGE_ALIGN_RIGHT
};

enum EImageVerticalAlign {
	IMAGE_ALIGN_TOP = 0,
	IMAGE_ALIGN_BOTTOM
};

class EImageDesc
{
public:
	EImageDesc(std::string name, std::string sourceType, int x, int y);

	void setWidth(double width);
	void setHeight(double height);
	void setWidthRatio(double width);
	void setHeightRatio(double height);
	void setHorizontalAlign(EImageHorizontalAlign align);
	void setVerticalAlign(EImageVerticalAlign align);

	std::string getName();
	std::string getType();
	int getX();
	int getY();
	double getWidth();
	double getHeight();
	double getWidthRatio();
	double getHeightRatio();
	EImageHorizontalAlign getHorizontalAlign();
	EImageVerticalAlign getVerticalAlign();

protected:
	std::string name;
	std::string type;
	int pos_x;
	int pos_y;
	double width;
	double height;
	double width_ratio;
	double height_ratio;

	EImageHorizontalAlign h_align;
	EImageVerticalAlign v_align;
};

/*
 * Grid Descriptor class
 * This class describes layered-grid map structure.
 */

enum EGridLayerID {
	GRID_LAYER_UNMOVABLE = 0,
	GRID_LAYER_MOVABLE,
	GRID_LAYER_EVENT,
	GRID_LAYER_END /* The number of layers. Do not change this */
};

class EGridDesc
{
public:
	EGridDesc(int width, int height, int levels);
	virtual ~EGridDesc();

	void setGridValue(int level, int x, int y, unsigned short value);
	unsigned short getGridValue(int level, int x, int y);
	int getGridWidth();
	int getGridHeight();
	int getGridLevels();

protected:
	int width;
	int height;
	int levels;
	unsigned short ***gridArray;
};

/**
 * ESceneLayerDesc
 * 		It stores descriptor divided for each layer
 */
class ESceneLayerDesc
{
public:
	ESceneLayerDesc(std::string name);

	/* Should handle creation of resources with Resource manager */
	void addSpriteDesc(std::shared_ptr<ESpriteDesc> sprite_desc);
	void addImageDesc(std::shared_ptr<EImageDesc> image_desc);
	void addGridDesc(std::shared_ptr<EGridDesc> grid_desc);

	std::string getName();

	/* Export read-only resources */
	const std::vector<std::shared_ptr<ESpriteDesc>>& sprite_list;
	const std::vector<std::shared_ptr<EImageDesc>>& image_list;
	const std::vector<std::shared_ptr<EGridDesc>>& grid_list;

protected:
	std::string name;

	std::vector<std::shared_ptr<ESpriteDesc>> _sprite_list;
	std::vector<std::shared_ptr<EImageDesc>> _image_list;
	std::vector<std::shared_ptr<EGridDesc>> _grid_list;
};

/**
 * Scene Descriptor
 *
 * This class stores required resource data to allocate scene.
 *  - Stores scene layer
 *  - Stores position order between sprites
 */

class ESceneDesc
{
public:
	ESceneDesc(std::string name, ESceneType type);

	void appendLayerDesc(std::shared_ptr<ESceneLayerDesc> layer);

	std::string getName();
	ESceneType getType();

	const std::vector<std::shared_ptr<ESceneLayerDesc>>& layer_list;

protected:
	ESceneDesc();

	std::string name;
	ESceneType type;
	std::vector<std::shared_ptr<ESceneLayerDesc>> _layer_list;
};
