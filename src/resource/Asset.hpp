#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

#include "util/LogHelper.hpp"

namespace story {
namespace Resource {

/* Asset
 *
 * It has a definition to use raw resource(e.g. image : png, jpg, etc.) with a pre-defined way
 */
class Asset
{
public:
  Asset(std::string name, std::string type)
  : name(name), type(type) {}

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

  void setParent(std::shared_ptr<Asset> parent) {
    this->parent = parent;
  }
  std::shared_ptr<Asset> getParent() const {
    return parent;
  }
  void appendChild(std::shared_ptr<Asset> child) {
    children.push_back(child);
  }
  std::vector<std::shared_ptr<Asset>> getChildren() const {
    return children;
  }

protected:
  std::string name;
  std::string type;
  std::map<std::string, std::string> property;

  std::shared_ptr<Asset> parent;
  std::vector<std::shared_ptr<Asset>> children;
};

} /* namespace Resource */
} /* namespace story */
