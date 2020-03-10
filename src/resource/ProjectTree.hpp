#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

#include "resource/ProjectObject.hpp"

namespace story {
namespace Resource {

/*
 * ProjectTree
 * 
 * It stores require project assets with tree data structure.
 */
class ProjectTree
{
public:
  ProjectTree();
  ProjectTree(const ProjectTree &pt) = delete;

private:
  std::shared_ptr<Resource::ProjectObject> tree;
};

} /* namespace Resource */
} /* namespace story */
