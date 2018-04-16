#include <iostream>
#include <pugixml.hpp>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "scene/ESceneDesc.hpp"
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
					std::string itm_h_align(node.node().attribute("halign").value());
					std::string itm_v_align(node.node().attribute("valign").value());
					std::string itm_p_x(node.node().attribute("x").value());
					std::string itm_p_y(node.node().attribute("y").value());
					std::string itm_p_w(node.node().attribute("width").value());
					std::string itm_p_h(node.node().attribute("height").value());
					std::string itm_levels(node.node().attribute("levels").value());
					int p_x = 0, p_y = 0;
					int p_w = 0, p_h = 0;
					int p_levels = 0;
					bool width_percent = false;
					bool height_percent = false;
					try {
						if (itm_h_align.empty())
							itm_h_align = std::string("left");
						if (itm_v_align.empty())
							itm_v_align = std::string("top");

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
						if (itm_levels.empty() == false) {
							p_levels = std::stoi(itm_levels);
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

						if (itm_h_align.compare("right") == 0)
							imageDesc->setHorizontalAlign(IMAGE_ALIGN_RIGHT);
						if (itm_v_align.compare("bottom") == 0)
							imageDesc->setVerticalAlign(IMAGE_ALIGN_BOTTOM);

						layerDesc->addImageDesc(imageDesc);
					}
					else if (itm_node.compare("TileMap") == 0) {
						LOG_INFO("Width: [%d] / Height: [%d] / Levels: [%d]", p_w, p_h, p_levels);
						std::string delim = " ";
						std::string base_tile(node.node().attribute("source").value());

						/* Tile map data */
						std::shared_ptr<EGridDesc> gridDesc(new EGridDesc(p_w, p_h, p_levels));
						gridDesc->setBaseImage(base_tile);
						for (auto map_layer : node.node().children()) {
							/* Map layer */
							std::string map_layer_name(map_layer.attribute("name").value());
							std::string level_layer(map_layer.attribute("level").value());
							int level = 0;

							/* Tile layer level */
							if (false == level_layer.empty())
								level = std::stoi(level_layer);

							LOG_DBG("Grid Layer [%s] (L %02d)", map_layer_name.c_str(), level);
							for (auto raw_array : map_layer.children())
							{
								std::string pcdata(raw_array.text().get());
								LOG_DBG("   raw : %s", pcdata.c_str());

								/* TODO: Current logic sets each character into grid */
								std::vector<std::string> row_elems;
								std::stringstream ss(pcdata);
								std::string item;
								int s_idx = 0;
								while (std::getline(ss, item, '\n')) {
									if (item.length() == 0) continue;
									row_elems.push_back(item);
									/*
									LOG_DBG("[%02d] (%lu) %s",
											s_idx, item.length(), item.c_str());
									*/

									/* TEMP: Parse each 2 bytes */
									size_t start = 0U;
									size_t end = item.find(delim);
									int i = 0;
									while (end != std::string::npos)
									{
										std::string val = item.substr(start, end - start);
										start = end + delim.length();
										end = item.find(delim, start);
										int v = std::stoi(val);
										gridDesc->setGridValue(level, i, s_idx, v);
										i++;
									}
									if (item.length() - start >= 4) {
										std::string val = item.substr(start, std::string::npos);
										val = item.substr(start, item.length());
										int v = std::stoi(val);
										gridDesc->setGridValue(level, i, s_idx, v);
									}
									/* Line end */
									s_idx++;
								}
							}
						}
						layerDesc->addGridDesc(gridDesc);
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
