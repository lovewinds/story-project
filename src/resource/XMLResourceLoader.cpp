#include <iostream>

#include "Ecore.h"
#include "util/LogHelper.hpp"
#include "resource/XMLResourceLoader.h"

XMLResourceLoader::XMLResourceLoader()
{
}

XMLResourceLoader::~XMLResourceLoader()
{
}

void XMLResourceLoader::loadScene(std::string& res_file)
{
	pugi::xml_document doc;
	if (!doc.load_file("../res/sample_scene.xml")) {
		if (!doc.load_file("sample_scene.xml")) {
			LOG_ERR("Failed to open xml scene file !");
		}
	}

	try {
		/* Examples: select image items */
		pugi::xpath_node_set sel = doc.select_nodes(
				"/SceneRoot/Resources/Images/Image[@type='sprite']");
		std::cout << "Nodes:" << std::endl;
		for (pugi::xpath_node_set::const_iterator it = sel.begin(); it != sel.end(); ++it) {
			pugi::xpath_node node = *it;
			LOG_DBG("%s", node.node().attribute("src").value());
		}

		/* Examples: sprite selection */
		pugi::xpath_node_set spr = doc.select_nodes(
				"/SceneRoot/Resources/Sprites/Sprite/Index[text()<5]");
		std::cout << "\n["<< spr.size() << "] Sprites:" << std::endl;
		for (pugi::xpath_node_set::const_iterator it = spr.begin(); it != spr.end(); ++it) {
			pugi::xpath_node node = *it;
			LOG_DBG("%s", node.node().child_value());
		}

		/* Examples: linear sprite selection */
		pugi::xpath_node_set linear = doc.select_nodes(
				"/SceneRoot/Resources/Sprites/Sprite/Index[@linear='start' or @linear='end']");
		std::cout << "\n["<< linear.size() << "] Sprites:" << std::endl;
		for (pugi::xpath_node_set::const_iterator it = linear.begin(); it != linear.end(); ++it) {
			pugi::xpath_node node = *it;
			LOG_DBG("%s", node.node().child_value());
		}
	}
	catch (const pugi::xpath_exception& e) {
		LOG_ERR("Select failed: %s", e.what());
	}
}

void XMLResourceLoader::loadSprites(pugi::xml_document &doc)
{
	Ecore *core = Ecore::getInstance();
	
}