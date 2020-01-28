#pragma once

#include <string>

class story::Resource::EResourceManager;

namespace story {
namespace Core {

class ProjectLoaderInterface
{
public:
	ProjectLoaderInterface(story::Resource::EResourceManager* manager) {
		this->resManager = manager;
	}
	virtual ~ProjectLoaderInterface(){ }

	/* Load all resources information from resource archive */
	virtual bool loadProject(std::string& res_path) = 0;

protected:
	story::Resource::EResourceManager	*resManager;
};

} /* namespace Core */
} /* namespace story */
