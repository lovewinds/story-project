#pragma once

#include <string>

#include <pugixml.hpp>
#include "resource/EResourceLoaderInterface.hpp"

class EResourceManager;

class XMLResourceLoader : public EResourceLoaderInterface
{
public:
	XMLResourceLoader(EResourceManager* mgr);
	virtual ~XMLResourceLoader();

	/* Load all resources information from resource archive */
	virtual bool loadProject(std::string& res_path);

private:
	void loadSprites(pugi::xml_document &document);
	void loadCommonResources(pugi::xml_document &document);
	void loadScenes(pugi::xml_document &document);

	std::shared_ptr<story::Graphic::ESceneDesc> loadSceneDesc(
		pugi::xml_document &document,
		std::string scene_name, story::Graphic::LayerType scene_type);
};
