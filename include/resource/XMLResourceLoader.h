#pragma once

#include <resource/EResourceLoaderInterface.h>

class XMLResourceLoader : public EResourceLoaderInterface
{
public:
	XMLResourceLoader();
	~XMLResourceLoader();

	virtual void loadScene(const char* res_file);
};
