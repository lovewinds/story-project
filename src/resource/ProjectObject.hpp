#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>
#include "util/LogHelper.hpp"

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
  ProjectObject(std::string type)
    : type(type), name(), property(), children() {}
  ProjectObject(const ProjectObject &pt) = delete;

  bool operator!=(const ProjectObject &other) const {
      return !(*this == other);
  }
  bool operator==(const ProjectObject &other) const {
    if (this->name != other.name) return false;
    if (this->type != other.type) return false;
    
    // Property comparison
    auto iter = this->property.begin();
    while (iter != this->property.end()) {
      std::string _key = iter->first;
      std::string _val = iter->second;
      if (_val != other.get(_key)) return false;

      iter++;
    }

    return true;
  }

  std::string getType() { return type; }
  std::string getName() { return name; }

  void add(std::string key, std::string value) {
    property.insert(std::make_pair(key, value));
  }
  void remove(std::string key, std::string value) {
    property.erase(key);
  }
  std::string get(std::string key) const {
    return property.at(key);
  }
  int getInt(std::string key) const {
    int ret = 0;
    try {
      ret = std::stoi(property.at(key).c_str());
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
  void setChildren(const std::vector<std::shared_ptr<ProjectObject>> children) {
    this->children.assign(children.begin(), children.end());
  }
  std::vector<std::shared_ptr<ProjectObject>> getChildren() const {
    return children;
  }

  void display() {
    LOG_DBG("[Object] %s [%s]", name.c_str(), type.c_str());
    LOG_DBG("  Props ::");

    auto it = property.begin();
    // Iterate over the map using Iterator till end.
    while (it != property.end()) {
      LOG_DBG("    [%8s] [%s]", it->first.c_str(), it->second.c_str());
      it++;
    }

    auto ch = children.begin();
    while (ch != children.end()) {
      ch->get()->display();
      ch++;
    }
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
