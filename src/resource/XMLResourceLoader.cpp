#include <iostream>
#include <pugixml.hpp>

#include "util/LogHelper.hpp"
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
		/* Scene: Load main scene */
		std::shared_ptr<ESceneInfo> scene;
		pugi::xpath_node_set scene_sel = doc.select_nodes(
				"/SceneRoot/Scene[@name='main']");
		for (pugi::xpath_node_set::const_iterator it = scene_sel.begin(); it != scene_sel.end(); ++it) {
			pugi::xpath_node node = *it;
			std::string scene_name(node.node().attribute("name").value());

			scene = resManager->createScene(scene_name);
		}

		/* Load all image resources */	/* [@type='background'] */
		pugi::xpath_node_set sel = doc.select_nodes(
				"/SceneRoot/Scene/Resources/Images/Image");
		for (pugi::xpath_node_set::const_iterator it = sel.begin(); it != sel.end(); ++it) {
			pugi::xpath_node node = *it;
			std::string name(node.node().attribute("name").value());
			std::string image(node.node().attribute("src").value());
			std::string width(node.node().attribute("width").value());
			std::string height(node.node().attribute("height").value());
			unsigned int _width = 0;
			unsigned int _height = 0;
			try {
				_width = std::stoi(width);
				_height = std::stoi(height);
			}
			catch (std::exception &e) {
				LOG_ERR("   Invalid width/height [%s / %s]", width.c_str(), height.c_str());
				continue;
			}
			resManager->createImageResource(name, image, _width, _height);
		}

		/* TODO: Create sprite information instances */
		/* Examples: sprite selection */
		//"/SceneRoot/Scene/Resources/Sprites/Sprite/Index[text()<5]/..");
		pugi::xpath_node_set spr = doc.select_nodes(
				"/SceneRoot/Scene/Resources/Sprites/Sprite");
		LOG_INFO("[%lu] Sprites:", spr.size());
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
				LOG_ERR("   Invalid width/height [%s / %s]", width.c_str(), height.c_str());
				continue;
			}

			std::shared_ptr<ESpriteType> spriteType(new ESpriteType(name, source));
			bool res = spriteType->setCellInfo(_width, _height);
			if (false == res) {
				LOG_ERR("Failed to initiate Sprite Type");
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

		/* Load all scene layers */
		pugi::xpath_node_set sel_layer = doc.select_nodes(
				"/SceneRoot/Scene/Layer/*");
		LOG_INFO("[%lu] Layer items:", sel_layer.size());
		/* TEMPORARY */
		int idx = 0;
		for (pugi::xpath_node_set::const_iterator it = sel_layer.begin(); it != sel_layer.end(); ++it) {
			pugi::xpath_node node = *it;
			std::string itm_node(node.node().name());
			std::string itm_type(node.node().attribute("type").value());
			std::string itm_name(node.node().attribute("name").value());
			std::string itm_p_x(node.node().attribute("x").value());
			std::string itm_p_y(node.node().attribute("y").value());
			int p_x=0, p_y=0;
			try {
				p_x = std::stoi(itm_p_x);
				p_y = std::stoi(itm_p_y);
			}
			catch (std::exception &e) {
				LOG_ERR("   Invalid position x/y [%s / %s]. use default pos (0,0)",
				 	itm_p_x.c_str(), itm_p_y.c_str());
			}

			/* Below logic just prepare for allocation.
			 * Instance will be allocated when scene enters 'Play' state. */
			if (itm_node.compare("Sprite") == 0) {
				/* Sprite instance */
				LOG_DBG("Finding sprite type [%s]", itm_type.c_str());
				std::shared_ptr<ESpriteType> spriteType = resManager->getSpriteType(itm_type);
				if (spriteType) {
					auto s = spriteType->createSprite(itm_name, p_x, p_y);
					s->setIndex(idx+1);
					scene->addSprite(s);
					LOG_INFO("   [Sprite] %s prepared", s->getName().c_str());
				}
				idx++;
			}
		}

		/* Examples: linear sprite selection */
		/*pugi::xpath_node_set linear = doc.select_nodes(
		 *		"/SceneRoot/Scene/Resources/Sprites/Sprite/Index[@linear='start' or @linear='end']");
		 */
	}
	catch (const pugi::xpath_exception& e) {
		LOG_INFO("Select failed: %s", e.what());
		result = false;
	}

	return result;
}
