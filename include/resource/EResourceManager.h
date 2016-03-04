#pragma once

#include <resource/EResourceLoaderInterface.h>

class EResourceManager
{
public:
	EResourceManager();
	~EResourceManager();

	void loadResource(const char* res_file);

private:
	EResourceLoaderInterface *loader;
};
