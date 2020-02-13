#pragma once

#include <string>
#include <map>
#include <vector>
#include <unordered_map>

#include "graphic/descriptors/GridDescriptor.hpp"
#include "graphic/descriptors/ImageDescriptor.hpp"
#include "graphic/descriptors/SpriteDescriptor.hpp"
#include "resource/description/ObjectDescription.hpp"

namespace story {
namespace Graphic {

/**
 * SubLayerDesc
 * It stores descriptor divided for each layer
 */
class SubLayerDesc
{
public:
  SubLayerDesc(std::string name) :
      sprite_list(_sprite_list),
      image_list(_image_list),
      grid_list(_grid_list),
      obj_array(_obj_array)
  {
    this->name = name;
  }

  /* Should handle creation of resources with Resource manager */
  void addSpriteDesc(std::shared_ptr<ESpriteDesc> sprite_desc) {
    if (nullptr != sprite_desc) _sprite_list.push_back(sprite_desc);
  }
  void addImageDesc(std::shared_ptr<EImageDesc> image_desc) {
    if (nullptr != image_desc) _image_list.push_back(image_desc);
  }
  void addGridDesc(std::shared_ptr<EGridDesc> grid_desc) {
    if (nullptr != grid_desc) _grid_list.push_back(grid_desc);
  }
  void appendObjectDesc(std::shared_ptr<Resource::ObjectDescription> obj_desc) {
    if (nullptr != obj_desc) _obj_array.push_back(obj_desc);
  }


  std::string getName() { return name; }

  /* Export read-only resources */
  const std::vector<std::shared_ptr<ESpriteDesc>>& sprite_list;
  const std::vector<std::shared_ptr<EImageDesc>>& image_list;
  const std::vector<std::shared_ptr<EGridDesc>>& grid_list;
  const std::vector<std::shared_ptr<Resource::ObjectDescription>>& obj_array;

protected:
  std::string name;

  std::vector<std::shared_ptr<ESpriteDesc>> _sprite_list;
  std::vector<std::shared_ptr<EImageDesc>> _image_list;
  std::vector<std::shared_ptr<EGridDesc>> _grid_list;
  std::vector<std::shared_ptr<Resource::ObjectDescription>> _obj_array;
};

} /* namespace Graphic */
} /* namespace story */
