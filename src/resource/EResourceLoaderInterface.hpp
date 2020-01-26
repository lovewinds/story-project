#pragma once

#include <string>

class EResourceManager;

class EResourceLoaderInterface
{
public:
	EResourceLoaderInterface(EResourceManager* mgr);
	virtual ~EResourceLoaderInterface(){ }

	/* Load all resources information from resource archive */
	virtual bool loadProject(std::string& res_path) = 0;

protected:
	EResourceManager	*resManager;
};
