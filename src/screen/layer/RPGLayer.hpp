#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "screen/layer/ScreenLayer.hpp"
#include "graphic/texture/SpriteTexture.hpp"
#include "graphic/texture/ImageTexture.hpp"
#include "graphic/texture/TextTexture.hpp"
#include "graphic/texture/GridMapTexture.hpp"
#include "graphic/animation/EAnimation.hpp"
#include "graphic/animation/EGridMoveAnimation.hpp"
#include "graphic/descriptors/GridDescriptor.hpp"

namespace story {
namespace Screen {

class RPGLayer : public ScreenLayer
{
public:
  RPGLayer(std::string name);
  virtual ~RPGLayer();

  /* Primitive Scene actions */
  virtual void handleEvent(SDL_Event e);
  virtual void render();
  virtual void update(Uint32 currentTime, Uint32 accumulator);

  virtual bool addObject(std::shared_ptr<Graphic::Object> object);

  virtual void setMap(std::shared_ptr<Graphic::GridMapTexture> map);
  virtual void setGridDescriptor(std::shared_ptr<Graphic::EGridDesc> desc);

  void testAnimation(Graphic::AnimationState state);
  bool testRotate(int x, int y);

  void handleDirectonFactor(float axis_x, float axis_y);
  bool checkGridMoveable(int x, int y);

  /* Callback when object is moved */
  void objectPositionCallback(double x, double y);

protected:
  std::shared_ptr<Graphic::GridMapTexture>  gridMap;
  std::shared_ptr<Graphic::EGridDesc> gridDesc;

  /* Grid-based object list for rendering */
  //std::vector<std::list<std::shared_ptr<Graphic::Object>>> _vertical_obj_list;
  //std::map<std::string, std::shared_ptr<Graphic::Object>> _object_map;
};

} /* namespace Screen */
} /* namespace story */
