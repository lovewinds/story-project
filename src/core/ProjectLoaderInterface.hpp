#pragma once

#include <string>

class EResourceManager;

namespace story {
namespace Core {

class ProjectLoaderInterface
{
public:
	ProjectLoaderInterface(EResourceManager* manager) {
		this->resManager = manager;
	}
	virtual ~ProjectLoaderInterface(){ }

	/* Load all resources information from resource archive */
	virtual bool loadProject(std::string& res_path) = 0;

protected:
	EResourceManager	*resManager;
};

} /* namespace Core */
} /* namespace story */
