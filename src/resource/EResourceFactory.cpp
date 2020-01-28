#include <SDL.h>

#include <string>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EResourceFactory.hpp"
#include "object/GraphicObject.hpp"

#include "scene/ERPGScene.hpp"
#include "scene/EMapScene.hpp"
#include "scene/EVisualNovelScene.hpp"
#include "scene/EDbgOverlayScene.hpp"

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
std::shared_ptr<Layer> EResourceFactory::createScene(std::string scene_name)
{
	std::shared_ptr<Layer> scene = nullptr;

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
			std::shared_ptr<story::Graphic::Object> object
				(new story::Graphic::Object());
			LOG_DBG("    Sprite [%s]", spriteDesc->getName().c_str());

			/* TODO: Creation should be performed with ObjectFactory */
			sprite = createSprite(spriteDesc);
			object->setName(spriteDesc->getName());
			object->movePositionTo(spriteDesc->getX(), spriteDesc->getY());
			if (spriteDesc->isControllable())
				object->setControllable(true);

			object->addSprite(sprite);
			scene->addObject(object);
			//scene->addSprite(sprite);
		}

		/* Create images from image descriptor */
		for (auto& image_it : layer->image_list)
		{
			std::shared_ptr<EImageDesc> imageDesc = image_it;
			std::shared_ptr<EImageTexture> image;
			std::shared_ptr<story::Graphic::Object> object
				(new story::Graphic::Object());
			LOG_DBG("    Image [%s]", imageDesc->getName().c_str());

			object->setName(imageDesc->getName());
			image = createImageTexture(imageDesc);
			if (nullptr == image) {
				LOG_ERR("Failed to create Image !");
			} else {
				auto h_align = imageDesc->getHorizontalAlign();
				auto v_align = imageDesc->getVerticalAlign();
				int px = imageDesc->getX();
				int py = imageDesc->getY();
				double width = 0.0f;
				double height = 0.0f;

				/* TODO: Inefficient logic */
				if (imageDesc->getWidthRatio() != 0.0) {
					image->setWidth(imageDesc->getWidthRatio(), true);
					LOG_ERR("Width [%d] / Ratio [%lf]", image->getWidth(), imageDesc->getWidthRatio());
					width = (imageDesc->getWidthRatio() / 100.0) * image->getWidth();
					LOG_ERR("Calculated Width [%lf]", width);
				}
				else {
					image->setWidth(imageDesc->getWidth(), false);
					width = image->getWidth();
				}
				if (imageDesc->getHeightRatio() != 0.0) {
					image->setHeight(imageDesc->getHeightRatio(), true);
					height = (imageDesc->getHeightRatio() / 100.0) * image->getHeight();
				}
				else {
					image->setHeight(imageDesc->getHeight(), false);
					height = image->getHeight();
				}

				if (imageDesc->getHorizontalAlign() == IMAGE_ALIGN_RIGHT) {
					px = (Ecore::getScreenWidth() - (int)width) - px;
				}
				if (imageDesc->getVerticalAlign() == IMAGE_ALIGN_BOTTOM) {
					py = (Ecore::getScreenHeight() - (int)height) + py;
				}

				object->movePositionTo(px, py);

				object->addImage(image);
				scene->addObject(object);
				//scene->addImage(image);
			}
		}

		/* Create images from image descriptor */
		for (auto& grid_it : layer->grid_list)
		{
			std::shared_ptr<EGridDesc> gridDesc = grid_it;
			std::string base_tile_image = "MapTile";
			base_tile_image = gridDesc->getBaseImage();

			/* RECOMMEND: Do not use dynamic_cast */
			ERPGScene* rpg_scene = dynamic_cast<ERPGScene*>(scene.get());
			EMapScene* map_scene = dynamic_cast<EMapScene*>(scene.get());
			std::shared_ptr<EGridMapTexture> map(new EGridMapTexture("MyMap", base_tile_image.c_str(), gridDesc));
			if (rpg_scene) {
				rpg_scene->setMap(map);
				rpg_scene->setGridDescriptor(gridDesc);
			}
			else if (map_scene) {
				map_scene->setMap(map);
				map_scene->setGridDescriptor(gridDesc);
			}
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
		SDL_Color textColor, SDL_Color bgColor,
		std::shared_ptr<SDLFontWrap> font)
{
	/* If not provided, create a default one */
	if (nullptr == font) {
		EResourceManager& resManager = Ecore::getInstance()->getResourceManager();

		font = resManager.getFont("NanumGothic", 28);
		if (nullptr == font) {
			LOG_ERR("Failed to get font[%s] size[%d]", "NanumGothic", 28);
			return nullptr;
		}
	}

	/* Currently, text textures are not cached */
	std::shared_ptr<SDLTextureWrap> texture(
		new SDLTextureWrap(text, textColor, bgColor, font));

	LOG_DBG("[ResFactory] texture [%p]", &texture);
	return texture;
}
