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
class EImageResource
{
public:
	EImageResource(std::string name, std::string path, unsigned int width, unsigned int height);
	virtual ~EImageResource();

	std::string getName() const;
	std::string getPath() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;

	std::shared_ptr<SDLTextureWrap>	getTexture();
	void releaseTexture();

protected:
	std::string name;
	std::string path;
	unsigned int width;
	unsigned int height;
	std::shared_ptr<SDLTextureWrap>	texture;
};
