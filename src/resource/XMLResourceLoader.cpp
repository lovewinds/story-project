#include <iostream>
#include <pugixml.hpp>

#include "resource/XMLResourceLoader.h"

XMLResourceLoader::XMLResourceLoader()
{
}

XMLResourceLoader::~XMLResourceLoader()
{
}

void XMLResourceLoader::loadScene(const char* res_file)
{
	pugi::xml_document doc;
	if (!doc.load_file("../res/sample_scene.xml")) {
		if (!doc.load_file("sample_scene.xml")) {
			std::cout << "Failed to open xml scene file !" << std::endl;
		}
	}

	try {
		/* Examples: select image items */
		pugi::xpath_node_set sel = doc.select_nodes(
				"/SceneRoot/Resources/Images/Image[@type='sprite']");
		std::cout << "Nodes:" << std::endl;
		for (pugi::xpath_node_set::const_iterator it = sel.begin(); it != sel.end(); ++it) {
			pugi::xpath_node node = *it;
			std::cout << node.node().attribute("src").value() << "\n";
		}

		/* Examples: sprite selection */
		pugi::xpath_node_set spr = doc.select_nodes(
				"/SceneRoot/Resources/Sprites/Sprite/Index[text()<5]");
		std::cout << "\n["<< spr.size() << "] Sprites:" << std::endl;
		for (pugi::xpath_node_set::const_iterator it = spr.begin(); it != spr.end(); ++it) {
			pugi::xpath_node node = *it;
			std::cout << node.node().child_value() << "\n";
		}

		/* Examples: linear sprite selection */
		pugi::xpath_node_set linear = doc.select_nodes(
				"/SceneRoot/Resources/Sprites/Sprite/Index[@linear='start' or @linear='end']");
		std::cout << "\n["<< linear.size() << "] Sprites:" << std::endl;
		for (pugi::xpath_node_set::const_iterator it = linear.begin(); it != linear.end(); ++it) {
			pugi::xpath_node node = *it;
			std::cout << node.node().child_value() << "\n";
		}
	}
	catch (const pugi::xpath_exception& e) {
		std::cout << "Select failed: " << e.what() << std::endl;
	}
}
