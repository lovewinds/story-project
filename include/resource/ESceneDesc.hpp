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

	std::string getName();
	std::string getType();
	int getX();
	int getY();

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

	std::string getName();
	std::string getType();
	int getX();
	int getY();

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
 * 		It stores descriptor divided for each layer
 */
class ESceneLayerDesc
{
public:
	ESceneLayerDesc(std::string name);

	/* Should handle creation of resources with Resource manager */
	void addSpriteDesc(std::shared_ptr<ESpriteDesc> sprite_desc);
	void addImageDesc(std::shared_ptr<EImageDesc> image_desc);

	/* Export read-only resources */
	const std::vector<std::shared_ptr<ESpriteDesc>>& sprite_list;
	const std::vector<std::shared_ptr<EImageDesc>>& image_list;

protected:
	std::string name;

	std::vector<std::shared_ptr<ESpriteDesc>> _sprite_list;
	std::vector<std::shared_ptr<EImageDesc>> _image_list;
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
