#pragma once

#include <string>
#include <memory>
#include <vector>

#include "util/SDLWrap.hpp"

/* TODO: Inherit serializable class */
/*
 * This class stores only resource information for actual allocation.
 * This class is stored in ResourceManager.
 * When scene is changed, SceneManager allocates actual Sprite instance
 * with this instance on ResourceManager.
 */
class ESpriteType
{
public:
	ESpriteType(std::string name, std::string base_image);
	virtual ~ESpriteType();

	bool setCellInfo(unsigned int cell_width, unsigned int cell_height);

	/* Store sprite cell information to create */
	bool appendSpriteCell(unsigned int index);

	//bool allocateSprite(int startIndex, int endIndex);
	//bool allocateSprite(std::vector<int> indexList);

	std::string getName();
	std::string getBaseImage();
	unsigned int getWidth() const;
	unsigned int getHeight() const;

	const std::vector<SDL_Rect>& cells;

protected:
	std::string name;
	std::string base_image;

	std::vector<SDL_Rect>	_cell;

	/* Width and Height of each cell */
	unsigned int cell_width;
	unsigned int cell_height;

	/* Width and Height of base image */
	unsigned int base_width;
	unsigned int base_height;
};
