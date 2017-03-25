#include <iostream>
#include <pugixml.hpp>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/ESceneDesc.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/XMLResourceLoader.hpp"

XMLResourceLoader::XMLResourceLoader(EResourceManager* mgr)
	: EResourceLoaderInterface(mgr)
{
}

XMLResourceLoader::~XMLResourceLoader()
{
	/* Release all resources */
}

bool XMLResourceLoader::loadScene(std::string& scene_name)
{
	return true;
}

bool XMLResourceLoader::loadResources(std::string& res_path)
{
	bool result = true;
	pugi::xml_document doc;
	if (!doc.load_file(res_path.c_str())) {
		std::cout << "Failed to open xml scene file !" << std::endl;
		return false;
	}

	/* TODO: Foreach for all scenes */
	try {
		std::stringstream s;

		/* Load Global image resources */
		s.str(std::string());
		s.clear();
		s << "/SceneRoot/GlobalResources/ImageResourceGroup/ImageResource";
		std::string img_path = s.str();
		pugi::xpath_node_set sel = doc.select_nodes(img_path.c_str());
		for (pugi::xpath_node_set::const_iterator it = sel.begin(); it != sel.end(); ++it) {
			pugi::xpath_node node = *it;
			std::string name(node.node().attribute("name").value());
			std::string image(node.node().attribute("path").value());
			std::string width(node.node().attribute("width").value());
			std::string height(node.node().attribute("height").value());
			unsigned int _width = 0;
			unsigned int _height = 0;
			try {
				_width = std::stoi(width);
				_height = std::stoi(height);
			}
			catch (std::exception &e) {
				LOG_ERR("%s", e.what());
				LOG_ERR("   Invalid width/height [%s / %s]", width.c_str(), height.c_str());
				continue;
			}
			resManager->createImageResource(name, image, _width, _height);
		}

		/* Load SpriteType Templete */
		//"/SceneRoot/Scene/Resources/Sprites/Sprite/Index[text()<5]/..");
		s.str(std::string());
		s.clear();
		s << "/SceneRoot/GlobalResources/SpriteTypeGroup/SpriteType";
		std::string sprite_path = s.str();
		pugi::xpath_node_set spr = doc.select_nodes(sprite_path.c_str());
		LOG_INFO("[%lu] SpriteTypes:", spr.size());
		for (pugi::xpath_node_set::const_iterator it = spr.begin(); it != spr.end(); ++it) {
			pugi::xpath_node node = *it;
			std::string name(node.node().attribute("name").value());
			std::string source(node.node().attribute("source").value());
			std::string width(node.node().attribute("width").value());
			std::string height(node.node().attribute("height").value());
			unsigned int _width = 0;
			unsigned int _height = 0;
			try {
				_width = std::stoi(width);
				_height = std::stoi(height);
			}
			catch (std::exception &e) {
				LOG_ERR("%s", e.what());
				LOG_ERR("   Invalid width/height [%s / %s]", width.c_str(), height.c_str());
				continue;
			}

			std::shared_ptr<ESpriteType> spriteType(new ESpriteType(name, source));
			bool res = spriteType->setCellInfo(_width, _height);
			if (false == res) {
				LOG_ERR("Failed to initiate SpriteType");
				continue;
			}
			for (pugi::xml_node idx : node.node().children())
			{
				std::string idxNumber(idx.text().get());
				if (idxNumber.empty()) continue;
				int val = atoi(idxNumber.c_str());
				LOG_DBG("   Index : %s", idxNumber.c_str());
				spriteType->appendSpriteCell(val);
			}
			resManager->createSpriteType(spriteType);
		}

		/* Create Scene descriptor */
		//pugi::xpath_node_set scene_sel = doc.select_nodes("/SceneRoot/Scene[@name='main']");
		pugi::xpath_node_set scene_sel = doc.select_nodes("/SceneRoot/Scene");
		for (auto scene_it = scene_sel.begin(); scene_it != scene_sel.end(); ++scene_it) {
			std::shared_ptr<ESceneDesc> sceneDesc = nullptr;
			ESceneType sceneType;
			pugi::xpath_node node = *scene_it;
			std::string scene_name(node.node().attribute("name").value());
			std::string scene_type(node.node().attribute("type").value());

			if (scene_type == "RPG")
				sceneType = SCENE_RPG;
				//scene = resManager->createScene(SCENE_RPG, scene_name);
			else if (scene_type == "VNovel")
				sceneType = SCENE_VISUAL_NOVEL;
				//scene = resManager->createScene(SCENE_VISUAL_NOVEL, scene_name);
			else if (scene_type == "Title")
				sceneType = SCENE_TITLE;
			else {
				LOG_ERR("Undefined scene type [%s]", scene_type.c_str());
				continue;
			}

			sceneDesc = std::shared_ptr<ESceneDesc>(new ESceneDesc(scene_name, sceneType));
			if (nullptr == sceneDesc) {
				LOG_ERR("Failed to create Scene descriptor");
				continue;
			}

			/* Load all scene layers */
			s.str(std::string());
			s.clear();
			s << "/SceneRoot/Scene[@name='" << scene_name << "']/Layer";
			std::string layer_path = s.str();
			pugi::xpath_node_set layer_sel = doc.select_nodes(layer_path.c_str());
			for (auto layer_it = layer_sel.begin(); layer_it != layer_sel.end(); ++layer_it) {
				/* Load all layer items */
				pugi::xpath_node layer_node = *layer_it;
				std::string layer_name(layer_node.node().attribute("name").value());

				/* Create Layer descriptor */
				std::shared_ptr<ESceneLayerDesc> layerDesc(new ESceneLayerDesc(layer_name));

				/* Layer contents */
				int idx = 0;

				s.str(std::string());
				s.clear();
				s << "/SceneRoot/Scene[@name='" << scene_name <<
						"']/Layer[@name='" << layer_name << "']/*";
				std::string layer_item_path = s.str();
				pugi::xpath_node_set layer_items = doc.select_nodes(layer_item_path.c_str());
				LOG_INFO("Layer[%s] has [%lu] items:",
						layer_name.c_str(), layer_items.size());

				/* auto = pugi::xpath_node_set::const_iterator */
				for (auto layer_items_it = layer_items.begin(); layer_items_it != layer_items.end(); ++layer_items_it) {
					pugi::xpath_node node = *layer_items_it;
					std::string itm_node(node.node().name());
					std::string itm_source(node.node().attribute("source").value());
					std::string itm_name(node.node().attribute("name").value());
					std::string itm_p_x(node.node().attribute("x").value());
					std::string itm_p_y(node.node().attribute("y").value());
					std::string itm_p_w(node.node().attribute("width").value());
					std::string itm_p_h(node.node().attribute("height").value());
					int p_x = 0, p_y = 0;
					int p_w = 0, p_h = 0;
					bool width_percent = false;
					bool height_percent = false;
					try {
						if (itm_p_x.empty() == false) {
							p_x = std::stoi(itm_p_x);
						}
						if (itm_p_y.empty() == false) {
							p_y = std::stoi(itm_p_y);
						}
						if (itm_p_w.empty() == false) {
							p_w = std::stoi(itm_p_w);
							if (itm_p_w.length() > 1 && itm_p_w.substr(itm_p_w.length() - 1, 1) == std::string("%"))
								width_percent = true;
						}
						if (itm_p_h.empty() == false) {
							p_h = std::stoi(itm_p_h);
							if (itm_p_h.length() > 1 && itm_p_h.substr(itm_p_h.length() - 1, 1) == std::string("%"))
								height_percent = true;
						}
					}
					catch (std::exception &e) {
						LOG_ERR("%s", e.what());
						LOG_ERR("   Invalid position x/y [%s / %s]. use default (0,0)",
							itm_p_x.c_str(), itm_p_y.c_str());
					}

					/* Below logic just prepare for allocation.
					 * Instance will be allocated when scene class is created. */
					if (itm_node.compare("Sprite") == 0) {
						/* Sprite descriptor */
						std::string itm_ctrl(node.node().attribute("controllable").value());
						LOG_INFO("Create Sprite descriptor [%s](T:%s) into (%3d, %3d)",
							itm_name.c_str(), itm_source.c_str(), p_x, p_y);
						std::shared_ptr<ESpriteDesc> spriteDesc(
							new ESpriteDesc(itm_name, itm_source, p_x, p_y)
						);

						if (itm_ctrl.empty() == false) {
							if (itm_ctrl.compare("true") == 0 || itm_ctrl.compare("1") == 0) {
								spriteDesc->setControllable(true);
							}
						}

						layerDesc->addSpriteDesc(spriteDesc);
					}
					else if (itm_node.compare("Image") == 0) {
						/* Image descriptor */
						LOG_INFO("Create Image descriptor [%s](T:%s) into (%3d, %3d)",
							itm_name.c_str(), itm_source.c_str(), p_x, p_y);
						std::shared_ptr<EImageDesc> imageDesc(
							new EImageDesc(itm_name, itm_source, p_x, p_y)
						);
						if (width_percent)
							imageDesc->setWidthRatio(p_w);
						else
							imageDesc->setWidth(p_w);
						if (height_percent)
							imageDesc->setHeightRatio(p_h);
						else
							imageDesc->setHeight(p_h);

						layerDesc->addImageDesc(imageDesc);
					}
					else if (itm_node.compare("RawMap") == 0) {
						LOG_INFO("Width: [%d] / Height: [%d]", p_w, p_h);
						/* Raw map data */
						for (pugi::xml_node map_idx : node.node().children())
						{
							std::string pcdata(map_idx.text().get());
							LOG_DBG("   raw : %s", pcdata.c_str());
						}
					}
				} /* Layer items loop */

				/* Add layer descriptor into scene descriptor */
				sceneDesc->appendLayerDesc(layerDesc);
			}
			/* Store scene descriptor into Resource manager */
			resManager->addSceneDesc(sceneDesc->getName(), sceneDesc);
		} /* Scene loop */

		/* Examples: linear sprite selection */
		/*pugi::xpath_node_set linear = doc.select_nodes(
		 *		"/SceneRoot/Scene/Resources/Sprites/Sprite/Index[@linear='start' or @linear='end']");
		 */
	}
	catch (const pugi::xpath_exception& e) {
		LOG_INFO("Scene creation failed: %s", e.what());
		result = false;
	}

	return result;
}
