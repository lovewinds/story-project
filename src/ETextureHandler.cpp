#include "ETextureHandler.h"


ETextureHandler::ETextureHandler()
{
}


ETextureHandler::~ETextureHandler()
{
}

void ETextureHandler::createTexture(int x, int y)
{
	static int maxCount = 0;
	if (maxCount == 0) {
		ETexture*	texture = new ETexture();
		textureList.push_back(texture);
	}
}

void ETextureHandler::removeTexture()
{
	textureList.clear();
}

void ETextureHandler::startTextureAnimation()
{

}