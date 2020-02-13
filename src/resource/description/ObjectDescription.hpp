#pragma once

#include <map>
#include <string>

namespace story {
namespace Resource {

class ObjectDescription
{
public:
  ObjectDescription() {}

  void add(std::string key, std::string value) {
    property.insert(std::make_pair(key, value));
  }
  void remove(std::string key, std::string value) {
    property.erase(key);
  }
  std::string get(std::string key) {
    return property[key];
  }

protected:
  std::string name;
  std::map<std::string, std::string> property;
};

} /* namespace Resource */
} /* namespace story */
