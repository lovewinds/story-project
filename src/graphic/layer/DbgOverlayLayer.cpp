#if !defined(PLATFORM_WINDOWS)
#pragma execution_character_set("utf-8")
#endif

#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "graphic/GraphicObject.hpp"

#include "graphic/layer/DbgOverlayLayer.hpp"

namespace story {
namespace Graphic {

DbgOverlayLayer::DbgOverlayLayer(std::string name)
 : Layer(name)
{
	LOG_INFO("DbgOverlayLayer[%s] created", name.c_str());

	EResourceManager& resManager = story::Core::Ecore::getInstance()->getResourceManager();
	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	SDL_Color bgColor = { 0x0, 0x0, 0x0 };
	std::shared_ptr<ETextTexture> tt(new ETextTexture("FPS: 0.0", textColor, bgColor, 20));
	std::shared_ptr<story::Graphic::Object> object(new story::Graphic::Object());

	object->setName("FPS");
	object->movePositionTo(20, 60);
	object->addText(tt);

	addObject(object);
}

DbgOverlayLayer::~DbgOverlayLayer()
{
	LOG_INFO("DbgOverlayLayer[%s] removed", name.c_str());
}

void DbgOverlayLayer::handleEvent(SDL_Event e)
{
}

void DbgOverlayLayer::render()
{
	for (auto &it : _img_texture_map)
	{
		it.second->render(0, 0, 0);
	}

	for (auto& it : _sprite_map)
	{
		it.second->render(0, 0, 0);
	}

	for (auto& it : _text_texture_map)
	{
		it.second->render(0, 0, 0);
	}

	for (auto& it : _drawable_map)
	{
		it.second->render(0, 0, 0);
	}

	for (auto& it : _object_map)
	{
		it.second->render();
	}
}

void DbgOverlayLayer::update(Uint32 currentTime, Uint32 accumulator)
{
	if (false == isActivated)
		return;

	/* Handle Text Texture */
	static char str[256] = { 0, };

	/* Update Text */
	double d_fps = story::Core::Ecore::getInstance()->GetFPS();
	SDL_snprintf(str, 256, "FPS: %0.2f", d_fps);

	for (auto& it : _sprite_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for (auto &it : _img_texture_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for (auto& it : _text_texture_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for (auto& it : _drawable_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for (auto& it : _object_map)
	{
		if (it.first == "FPS") {
			it.second->updateText(str);
		}
		it.second->update(currentTime, accumulator);
	}

}

} /* namespace Graphic */
} /* namespace story */
