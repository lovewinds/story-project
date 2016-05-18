#pragma once

#include <string>
#include "util/SDLWrap.hpp"

/* TODO: Inherit serializable class */
/*
 * This class stores only resource information for actual allocation.
 * All of these ~~Info classes are stored in ResourceManager.
 * When scene is changed, SceneManager allocates actual Scene instance
 * with this ~~Info instance on ResourceManager.
 */
class EImageResourceInfo
{
public:
	EImageResourceInfo(std::string name, std::string path, unsigned int width, unsigned int height);
	virtual ~EImageResourceInfo();

	std::string getName() const;
	std::string getPath() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;

	void allocate();

	/* CHECK: Should provide it as weak_ptr? - to sprite instance */
	std::shared_ptr<SDL_Texture_Wrap>	getTexture();

protected:
	std::string name;
	std::string path;
	unsigned int width;
	unsigned int height;
	std::shared_ptr<SDL_Texture_Wrap>	texture;
};
