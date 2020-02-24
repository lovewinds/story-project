#pragma once

#include <map>
#include <string>

namespace story {
namespace Resource {

class ProjectItem
{
public:
  ProjectItem() {}

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
      ret = 0;
    }
    return ret;
  }

protected:
  std::string name;
  std::map<std::string, std::string> property;
};

} /* namespace Resource */
} /* namespace story */
