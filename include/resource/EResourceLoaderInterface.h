#pragma once

class EResourceLoaderInterface
{
public:
	virtual ~EResourceLoaderInterface() {}

	virtual void loadScene(const char* res_file) = 0;
};
