#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

namespace story {
namespace Resource {

/*
 * ProjectObject
 * 
 * It has a definition to use raw resource(e.g. image : png, jpg, etc.)
 * with a pre-defined way.
 */
class ProjectObject
{
public:
  ProjectObject(std::string type, std::string name): type(type), name(name) {}
  ProjectObject(const ProjectObject &pt) = delete;

  void add(std::string key, std::string value) {
    property.insert(std::make_pair(key, value));
  }
  void remove(std::string key, std::string value) {
    property.erase(key);
  }
  std::string get(std::string key) {
    return property[key];
  }
  int getInt(std::string key) {
    int ret = 0;
    try {
      ret = std::stoi(property[key].c_str());
    }
    catch (std::exception e) {
      LOG_ERR("Invalid key [%s]", key.c_str());
      ret = 0;
    }
    return ret;
  }

  // Node-handling functions
  void setParent(std::shared_ptr<ProjectObject> parent) {
    this->parent = parent;
  }
  std::shared_ptr<ProjectObject> getParent() const {
    return parent;
  }
  void appendChild(std::shared_ptr<ProjectObject> child) {
    children.push_back(child);
  }
  std::vector<std::shared_ptr<ProjectObject>> getChildren() const {
    return children;
  }

protected:
  std::string type;
  std::string name;
  std::map<std::string, std::string> property;

  // Tree node variables
  std::shared_ptr<ProjectObject> parent;
  std::vector<std::shared_ptr<ProjectObject>> children;
};

} /* namespace Resource */
} /* namespace story */
