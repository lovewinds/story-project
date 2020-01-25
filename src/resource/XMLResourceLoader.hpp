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
	virtual bool loadResources(std::string& res_path);

	/* Load specific scene and related resource(e.g. sprites) into memory */
	virtual bool loadScene(std::string& scene_name);

private:
	void loadSprites(pugi::xml_document &xml_document);
};
