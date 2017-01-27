#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <memory>

#include <SDL.h>

#include "resource/EScene.hpp"
#include "resource/ESceneDesc.hpp"
#include "resource/ESpriteType.hpp"
#include "resource/EImageResource.hpp"
#include "texture/ESprite.hpp"

class EResourceLoaderInterface;

class EResourceFactory
{
public:
	EResourceFactory();
	~EResourceFactory();

	/* Prohibit copy constructor */
	EResourceFactory(EResourceFactory& self) = delete;

/*
 * Scene Functions
 */
	/* Create scene from stored scene descriptor */
	std::shared_ptr<EScene> createScene(std::string scene_name);

	/* Remove and de-allocate specific scene
	 * and related resource(i.e. sprites) from memory */
	void removeScene(std::string scene_name);

/*
 * Image texture Functions
 */
	std::shared_ptr<EImageTexture>
	createImageTexture(std::shared_ptr<EImageDesc> imageDesc);

	void removeImageTexture(std::string name);

/*
 * Sprite Functions
 */
	std::shared_ptr<ESprite>
	createSprite(std::shared_ptr<ESpriteDesc> spriteDesc);

	void removeSprite(std::string name);

/**
 * Low - layer functions
 */
	std::shared_ptr<SDLTextureWrap>
	createTextTexture(std::string text, SDL_Color textColor, SDL_Color bgColor);

};
