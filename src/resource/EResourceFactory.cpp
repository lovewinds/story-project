#include <SDL.h>

#include <string>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EResourceFactory.hpp"

#include "resource/ERPGScene.hpp"
#include "resource/EVisualNovelScene.hpp"
#include "resource/EDbgOverlayScene.hpp"

EResourceFactory::EResourceFactory()
{
}

EResourceFactory::~EResourceFactory()
{
	LOG_DBG("Bye ResourceFactory !");
}

/*
 * Scene Functions
 */
std::shared_ptr<EScene> EResourceFactory::createScene(std::string scene_name)
{
	std::shared_ptr<EScene> scene = nullptr;

	/* Search scene desc */
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	std::shared_ptr<ESceneDesc> sceneDesc = resManager.getSceneDesc(scene_name);
	if (nullptr == sceneDesc) {
		LOG_ERR("Failed to find scene descriptor");
		return nullptr;
	}

	LOG_DBG("Creating scene [%s].....", sceneDesc->getName().c_str());
	scene = resManager.createScene(sceneDesc->getType(), scene_name);

	/* For each layer */
	for (auto& layer_iter : sceneDesc->layer_list)
	{
		std::shared_ptr<ESceneLayerDesc> layer = layer_iter;
		LOG_DBG("  Scene Layer  [%s]", layer->getName().c_str());

		/* Create sprites from sprite descriptor */
		for (auto& sprite_it : layer->sprite_list)
		{
			std::shared_ptr<ESpriteDesc> spriteDesc = sprite_it;
			std::shared_ptr<ESprite> sprite;
			LOG_DBG("    Sprite [%s]", spriteDesc->getName().c_str());

			sprite = createSprite(spriteDesc);
			sprite->movePositionTo(spriteDesc->getX(), spriteDesc->getY());
			scene->addSprite(sprite);
		}

		/* Create images from image descriptor */
		for (auto& image_it : layer->image_list)
		{
			std::shared_ptr<EImageDesc> imageDesc = image_it;
			std::shared_ptr<EImageTexture> image;
			LOG_DBG("    Image [%s]", imageDesc->getName().c_str());

			image = createImageTexture(imageDesc);
			if (imageDesc->getWidthRatio() != 0.0)
				image->setWidth(imageDesc->getWidthRatio(), true);
			else
				image->setWidth(imageDesc->getWidth(), false);
			if (imageDesc->getHeightRatio() != 0.0)
				image->setHeight(imageDesc->getHeightRatio(), true);
			else
				image->setHeight(imageDesc->getHeight(), false);
			image->movePositionTo(imageDesc->getX(), imageDesc->getY());
			scene->addImage(image);
		}
	}

	return scene;
}

void EResourceFactory::removeScene(std::string scene_name)
{
	/* TODO: Notify removal to resource manager to handle ref count */
}

/*
* Image texture Functions
*/
std::shared_ptr<EImageTexture>
EResourceFactory::createImageTexture(std::shared_ptr<EImageDesc> imageDesc)
{
	/* Search image desc */
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	std::shared_ptr<EImageTexture> image = nullptr;

	if (nullptr == imageDesc)
		return nullptr;

	image = resManager.createImageTexture(imageDesc->getName(), imageDesc->getType());

	return image;
}

void EResourceFactory::removeImageTexture(std::string name)
{

}

/*
* Sprite Functions
*/
std::shared_ptr<ESprite>
EResourceFactory::createSprite(std::shared_ptr<ESpriteDesc> spriteDesc)
{
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	std::shared_ptr<ESprite> sprite = nullptr;

	if (nullptr == spriteDesc)
		return nullptr;

	sprite = resManager.createSprite(spriteDesc->getType(), spriteDesc->getName());

	return sprite;
}

void EResourceFactory::removeSprite(std::string name)
{

}

/**
 * Low layer functions
 */
std::shared_ptr<SDLTextureWrap>
EResourceFactory::createTextTexture(std::string text,
		SDL_Color textColor, SDL_Color bgColor)
{
	/* Currently, text textures are not cached */
	std::shared_ptr<SDLTextureWrap> texture(
		new SDLTextureWrap(text, textColor, bgColor));

	LOG_DBG("[ResFactory] texture [%p]", &texture);
	return texture;
}
