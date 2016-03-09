#pragma once

#include <pugixml.hpp>
#include <resource/EResourceLoaderInterface.h>

class XMLResourceLoader : public EResourceLoaderInterface
{
public:
	XMLResourceLoader();
	~XMLResourceLoader();

	virtual void loadScene(std::string& res_file);

private:
	void loadSprites(pugi::xml_document &xml_document);
};
