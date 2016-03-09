#pragma once

#include <resource/EResourceLoaderInterface.h>

class EResourceManager
{
public:
	EResourceManager();
	~EResourceManager();

	void loadResource(std::string& res_file);

private:
	/* TODO: Get specific loader from factory class */
	EResourceLoaderInterface *loader;
};
