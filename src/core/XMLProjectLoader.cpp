#include <iostream>
#include <sstream>
#include <pugixml.hpp>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "graphic/descriptors/SceneDescripor.hpp"
#include "graphic/descriptors/SubLayerDescriptor.hpp"
#include "core/ProjectLoaderInterface.hpp"
#include "core/XMLProjectLoader.hpp"

namespace story {
namespace Core {

XMLResourceLoader::XMLResourceLoader(EResourceManager* mgr)
	: ProjectLoaderInterface(mgr)
{
}

XMLResourceLoader::~XMLResourceLoader()
{
	/* Release all resources */
}

void XMLResourceLoader::loadSprites(pugi::xml_document &document) {
	std::stringstream s;
	/* Load SpriteType Templete */
	//"/SceneRoot/Scene/Resources/Sprites/Sprite/Index[text()<5]/..");
	s.str(std::string());
	s.clear();
	s << "/SceneRoot/GlobalResources/SpriteTypeGroup/SpriteType";
	std::string sprite_path = s.str();
	pugi::xpath_node_set spr = document.select_nodes(sprite_path.c_str());
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
}

void XMLResourceLoader::loadCommonResources(pugi::xml_document &document) {
	std::stringstream s;
	/* Load Global image resources */
	s.str(std::string());
	s.clear();
	s << "/SceneRoot/GlobalResources/ImageResourceGroup/ImageResource";
	std::string img_path = s.str();
	pugi::xpath_node_set sel = document.select_nodes(img_path.c_str());
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
}

std::shared_ptr<story::Graphic::ESceneDesc>
XMLResourceLoader::loadSceneDesc(
		pugi::xml_document &document,
		std::string scene_name, story::Graphic::LayerType layer_type) {

	std::shared_ptr<story::Graphic::ESceneDesc> scene_desc
		= std::shared_ptr<story::Graphic::ESceneDesc>(new story::Graphic::ESceneDesc(scene_name, layer_type));

	/* Load all scene layers */
	std::stringstream s;
	s.str(std::string());
	s.clear();
	s << "/SceneRoot/Scene[@name='" << scene_name << "']/Layer";
	std::string layer_path = s.str();
	pugi::xpath_node_set layer_sel = document.select_nodes(layer_path.c_str());
	for (auto layer_it = layer_sel.begin(); layer_it != layer_sel.end(); ++layer_it) {
		/* Load all layer items */
		pugi::xpath_node layer_node = *layer_it;
		std::string layer_name(layer_node.node().attribute("name").value());

		/* Create SubLayer descriptor */
		std::shared_ptr<story::Graphic::SubLayerDesc> layerDesc(
			new story::Graphic::SubLayerDesc(layer_name)
		);

		/* Layer contents */
		int idx = 0;

		s.str(std::string());
		s.clear();
		s << "/SceneRoot/Scene[@name='" << scene_name << "']/Layer[@name='" << layer_name << "']/*";
		std::string layer_item_path = s.str();
		pugi::xpath_node_set layer_items = document.select_nodes(layer_item_path.c_str());
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
				std::shared_ptr<story::Graphic::ESpriteDesc> spriteDesc(
					new story::Graphic::ESpriteDesc(itm_name, itm_source, p_x, p_y)
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
				std::shared_ptr<story::Graphic::EImageDesc> imageDesc(
					new story::Graphic::EImageDesc(itm_name, itm_source, p_x, p_y)
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
					imageDesc->setHorizontalAlign(story::Graphic::IMAGE_ALIGN_RIGHT);
				if (itm_v_align.compare("bottom") == 0)
					imageDesc->setVerticalAlign(story::Graphic::IMAGE_ALIGN_BOTTOM);

				layerDesc->addImageDesc(imageDesc);
			}
			else if (itm_node.compare("TileMap") == 0) {
				LOG_INFO("Width: [%d] / Height: [%d] / Levels: [%d]", p_w, p_h, p_levels);
				std::string delim = " ";
				std::string base_tile(node.node().attribute("source").value());

				/* Tile map data */
				std::shared_ptr<story::Graphic::EGridDesc> gridDesc(
					new story::Graphic::EGridDesc(p_w, p_h, p_levels)
				);
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
		scene_desc->appendLayerDesc(layerDesc);
	}

	return scene_desc;
}

void XMLResourceLoader::loadScenes(pugi::xml_document &document) {
	/* Create Scene descriptor */
	//pugi::xpath_node_set scene_sel = document.select_nodes("/SceneRoot/Scene[@name='main']");
	pugi::xpath_node_set scene_sel = document.select_nodes("/SceneRoot/Scene");
	for (auto scene_it = scene_sel.begin(); scene_it != scene_sel.end(); ++scene_it) {
		std::shared_ptr<story::Graphic::ESceneDesc> sceneDesc = nullptr;
		story::Graphic::LayerType layer_type_object;
		pugi::xpath_node node = *scene_it;
		std::string scene_name(node.node().attribute("name").value());
		std::string layer_type(node.node().attribute("type").value());

		if (layer_type == "RPG") {
			layer_type_object = story::Graphic::LAYER_RPG;
			//scene = resManager->createScene(SCENE_RPG, scene_name);
		}
		else if (layer_type == "Map") {
			layer_type_object = story::Graphic::LAYER_MAP;
		}
		else if (layer_type == "VNovel") {
			layer_type_object = story::Graphic::LAYER_CHAT;
		}
		else if (layer_type == "Title") {
			layer_type_object = story::Graphic::LAYER_TITLE;
		}
		else {
			LOG_ERR("Undefined scene type [%s]", layer_type.c_str());
			continue;
		}

		sceneDesc = loadSceneDesc(document, scene_name, layer_type_object);
		if (nullptr == sceneDesc) {
			LOG_ERR("Failed to create Scene descriptor");
			continue;
		}

		/* Store scene descriptor into Resource manager */
		resManager->addSceneDesc(sceneDesc->getName(), sceneDesc);
	} /* Scene loop */
}

bool XMLResourceLoader::loadProject(std::string& res_path)
{
	bool result = true;
	pugi::xml_document doc;
	if (!doc.load_file(res_path.c_str())) {
		LOG_ERR("Failed to open xml scene file !");
		return false;
	}

	try {
		loadCommonResources(doc);
		loadSprites(doc);
		loadScenes(doc);
	}
	catch (const pugi::xpath_exception& e) {
		LOG_INFO("Scene creation failed: %s", e.what());
		result = false;
	}

	return result;
}

} /* namespace Core */
} /* namespace story */
