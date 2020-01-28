#pragma once

#include <string>

#include "graphic/descriptors/SubLayerDescriptor.hpp"

namespace story {
namespace Graphic {

enum LayerType
{
  LAYER_RPG = 0,
  LAYER_CHAT,
  LAYER_DEBUG,
  LAYER_TITLE,
  LAYER_MAP
};

/**
 * Scene Descriptor
 *
 * This class stores required resource data to allocate scene.
 *  - Stores scene layer
 *  - Stores position order between sprites
 */

class ESceneDesc
{
public:
  ESceneDesc(std::string name, LayerType type) :
      layer_list(_layer_list)
  {
    this->name = name;
    this->type = type;
  }

  void appendLayerDesc(std::shared_ptr<SubLayerDesc> layer) {
    if (nullptr != layer) _layer_list.push_back(layer);
  }

  std::string getName() { return name; }
  LayerType getType() { return type; }

  const std::vector<std::shared_ptr<SubLayerDesc>>& layer_list;

protected:
  ESceneDesc();

  std::string name;
  LayerType type;
  std::vector<std::shared_ptr<SubLayerDesc>> _layer_list;
};

} /* namespace Graphic */
} /* namespace story */
