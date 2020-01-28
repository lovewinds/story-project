#pragma once

#include <string>
#include "util/SDLWrap.hpp"

namespace story {
namespace Resource {

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

} /* namespace Resource */
} /* namespace story */
