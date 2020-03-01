#pragma once

#include <map>
#include <string>

namespace story {
namespace Resource {

class GlobalAssetRegistry
{
public:
  GlobalAssetRegistry();
  virtual ~GlobalAssetRegistry();

  void registerObject();
  std::string getPath() const;
  unsigned int getWidth() const;
  unsigned int getHeight() const;

  void releaseTexture();

protected:
  std::map<std::string, std::shared_ptr<std::string>> _registry;
};

} /* namespace Resource */
} /* namespace story */
