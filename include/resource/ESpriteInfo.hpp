#pragma once

#include <string>
#include <memory>
#include "util/SDLWrap.hpp"

/* TODO: Inherit serializable class */
/*
 * This class stores only resource information for actual allocation.
 * All of these ~~Info classes are stored in ResourceManager.
 * When scene is changed, SceneManager allocates actual Scene instance
 * with this ~~Info instance on ResourceManager.
 */
class ESpriteInfo
{
public:
	ESpriteInfo(std::string name, std::string image_res);
	virtual ~ESpriteInfo();

	void setSpriteCells();

	/* Allocate sprite with related image resource into memory */
	bool allocateSprite();
	//bool allocateSprite(int startIndex, int endIndex);
	//bool allocateSprite(std::vector<int> indexList);

	std::string getName();

protected:
	std::string name;
	std::string image_res;
	std::shared_ptr<SDL_Texture_Wrap>	texture;
};
