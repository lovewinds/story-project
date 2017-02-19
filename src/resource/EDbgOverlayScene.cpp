#pragma execution_character_set("utf-8")

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"

#include "resource/EDbgOverlayScene.hpp"

EDbgOverlayScene::EDbgOverlayScene(std::string name)
 : EScene(name)
{
	LOG_INFO("EDbgOverlayScene[%s] created", name.c_str());

	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	SDL_Color bgColor = { 0x0, 0x0, 0x0 };
	std::shared_ptr<ETextTexture> tt(new ETextTexture("FPS: 0.0", textColor, bgColor));

	SDL_Window* window = Ecore::getInstance()->getWindow();
	int width = 0, height = 0;
	SDL_GetWindowSize(window, &width, &height);
	tt->movePositionTo(width - 160, 10);
	

	auto result = _text_texture_map.emplace("fps", tt);
	if (!result.second) {
		LOG_ERR("Failed to insert text set !");
	}
}

EDbgOverlayScene::~EDbgOverlayScene()
{
	LOG_INFO("EDbgOverlayScene[%s] removed", name.c_str());
}

void EDbgOverlayScene::handleEvent(SDL_Event e)
{
}

void EDbgOverlayScene::render()
{
	for (auto &it : _img_texture_map)
	{
		it.second->render(0, 0);
	}

	for (auto& it : _sprite_map)
	{
		it.second->render(0, 0);
	}

	for (auto& it : _text_texture_map)
	{
		it.second->render(0, 0);
	}

	for (auto& it : _drawable_map)
	{
		it.second->render(0, 0);
	}
}

void EDbgOverlayScene::update(Uint32 currentTime, Uint32 accumulator)
{
	if (false == isActivated)
		return;

	/* Handle Text Texture */
	static char str[256] = { 0, };

	/* Update Text */
	double d_fps = Ecore::getInstance()->GetFPS();
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
		if (it.first == "fps") {
			it.second->setText(str);
		}
		it.second->update(currentTime, accumulator);
	}

	for (auto& it : _drawable_map)
	{
		it.second->update(currentTime, accumulator);
	}
}
