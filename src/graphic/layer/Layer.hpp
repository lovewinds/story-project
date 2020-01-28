#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "util/SDLWrap.hpp"
#include "texture/ESprite.hpp"
#include "texture/EImageTexture.hpp"
#include "texture/ETextTexture.hpp"
#include "texture/EFigure.hpp"
#include "object/GraphicObject.hpp"
#include "object/animation/EAnimation.hpp"

namespace story {
namespace Graphic {

/**
 * Layer
 *
 * This class indicates specific graphic layer on screen.
 *  - It should own textures to show.
 *  - It should perform render and update.
 */
class Layer
{
public:
	Layer(std::string name);
	virtual ~Layer();

	/* Primitive Scene actions */
	virtual void handleEvent(SDL_Event e);
	virtual void render();
	virtual void update(Uint32 currentTime, Uint32 accumulator);

	std::string getName();

	/* Should handle creation of resources with ResourceFactory */
	virtual bool addObject(std::shared_ptr<story::Graphic::Object> object);

	/* Resource manager allocates image resource */
	//std::shared_ptr<SDLTextureWrap> allocateTexture(std::string path);
	//void releaseTexture(std::string path);

	void setActiveState(bool active);

	/* Export read-only resources */
	const std::map<std::string, std::shared_ptr<ESprite>>& sprite_map;

protected:
	Layer();
	std::string name;

	/* TODO: Scene is activated, so it perform update and render */
	bool isActivated;

	/**
	 * Texture map
	 *   - All these pointers of texture resources are stored here
	 *     to repeated search logic in ResourceManager.
	 *   - Also, shared_ptr need to be stored here also
	 *     for increase ref count.
	 */
	std::map<std::string, std::shared_ptr<ESprite>> _sprite_map;

	std::map<std::string, std::shared_ptr<EImageTexture>> _img_texture_map;

	std::map<std::string, std::shared_ptr<ETextTexture>> _text_texture_map;

	std::map<std::string, std::shared_ptr<EFigure>> _drawable_map;

	/* Not classified textures */
	std::map<std::string, std::shared_ptr<EDrawable>> _raw_texture_map;

	std::map<std::string, std::shared_ptr<story::Graphic::Object>> _object_map;
};

} /* namespace Graphic */
} /* namespace story */
