#pragma once

#include <string>

class EResourceManager;

class EResourceLoaderInterface
{
public:
	EResourceLoaderInterface(EResourceManager* mgr);
	virtual ~EResourceLoaderInterface(){ }

	/* Load all resources information from resource archive */
	virtual bool loadResources(std::string& res_path) = 0;

	/* Load specific scene and related resource(e.g. sprites) into memory */
	virtual bool loadScene(std::string& scene_name) = 0;

protected:
	EResourceManager	*resManager;
};
