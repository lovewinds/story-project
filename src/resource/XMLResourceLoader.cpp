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

			resManager->createImageResource(name, image);
		}

		/* TODO: Create sprite information instances */
		/* Examples: sprite selection */
		pugi::xpath_node_set spr = doc.select_nodes(
				"/SceneRoot/Scene/Resources/Sprites/Sprite/Index[text()<5]/..");
		LOG_INFO("[%lu] Sprites:", spr.size());
		for (pugi::xpath_node_set::const_iterator it = spr.begin(); it != spr.end(); ++it) {
			pugi::xpath_node node = *it;
			//LOG_INFO("   %s", node.node().child_value());
			LOG_INFO("   %s", node.node().attribute("name").value());
			std::string name(node.node().attribute("name").value());
			std::string source(node.node().attribute("source").value());

			resManager->createSprite(name, source);
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
