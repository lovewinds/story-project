#include <iostream>

#include <pugixml.hpp>

int testXml()
{
	pugi::xml_document doc;
	if (!doc.load_file("../res/sample_scene.xml")) {
		std::cout << "Failed to open xml scene file !" << std::endl;
		return -1;
	}

	try {
		pugi::xpath_node_set sel = doc.select_nodes("/SceneRoot/Resources/Images/Image[@type='sprite']");
		std::cout << "Nodes:\n" << std::endl;
		for (pugi::xpath_node_set::const_iterator it = sel.begin(); it != sel.end(); ++it) {
			pugi::xpath_node node = *it;
			std::cout << node.node().attribute("src").value() << "\n";
		}

		pugi::xpath_node_set spr = doc.select_nodes("/SceneRoot/Resources/Sprites/Sprite/Index/text()[text()<3]");
		std::cout << "Sprites:\n" << std::endl;
		for (pugi::xpath_node_set::const_iterator it = spr.begin(); it != spr.end(); ++it) {
			pugi::xpath_node node = *it;
			std::cout << node.node().value() << "\n";
		}
	}
	catch (const pugi::xpath_exception& e) {
		std::cout << "Select failed: " << e.what() << std::endl;
	}
	return 0;
}
