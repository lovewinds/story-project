#pragma once

#include <iostream>

class EResourceLoaderInterface
{
public:
	virtual ~EResourceLoaderInterface() {}

	virtual void loadScene(std::string& res_file) = 0;
};
