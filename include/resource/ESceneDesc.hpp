#pragma once

#include <string>
#include <map>
#include <vector>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"
#include "texture/ETextTexture.hpp"
#include "texture/EAnimation.hpp"
#include "texture/EFigure.hpp"

/* TODO: Inherit serializable class */

enum ESceneType
{
	SCENE_RPG = 0,
	SCENE_VISUAL_NOVEL,
	SCENE_DEBUG,
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

protected:
	std::string name;
	std::string type;
	int pos_x;
	int pos_y;
};

class EImageDesc
{
public:
	EImageDesc(std::string name, std::string sourceType, int x, int y);

	void setWidth(int width);
	void setHeight(int height);
	void setWidthRatio(double width);
	void setHeightRatio(double height);

protected:
	std::string name;
	std::string type;
	int pos_x;
	int pos_y;
	int width;
	int height;
	double width_ratio;
	double height_ratio;
};

/**
 * ESceneLayerDesc
 * It stores 
 */
class ESceneLayerDesc
{
public:
	ESceneLayerDesc(std::string name);

	/* Should handle creation of resources with Resource manager */
	bool addSpriteDesc(std::string name, std::string sourceResource,
			int x, int y);

	bool addImageDesc(std::string name, std::string sourceResource,
			int x, int y,
			int width, int height,
			int width_percent, int height_percent);

protected:
	std::vector<std::shared_ptr<ESpriteDesc>> sprite_list;
	std::string name;
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
	virtual ~ESceneDesc();

	bool appendLayerDesc(std::shared_ptr<ESceneLayerDesc> layer);
	std::shared_ptr<ESceneLayerDesc>
	getLayerDesc(int index);

	std::string getName();

protected:
	ESceneDesc();

	std::string name;
	ESceneType type;
	std::vector<std::shared_ptr<ESceneLayerDesc>> layer_list;
};
