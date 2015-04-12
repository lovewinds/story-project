#pragma once
#include <list>
#include "Ecore.h"
#include "ETexture.h"

using std::list;

enum TextureType {
	TEXTURE_TYPE_FONT,
	TEXTURE_TYPE_PNG,
	TEXTURE_TYPE_RENDER,
};

class ETextureHandler
{
public:
	ETextureHandler();
	~ETextureHandler();

	static ETextureHandler getInstance() {
		if (instance == NULL) {

		}
	}

	void createTexture(int x, int y);
	void removeTexture();
	void startTextureAnimation();

private:
	static ETextureHandler*	instance;
	list<ETexture*>	textureList;
};
