#pragma execution_character_set("utf-8")

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "texture/ESprite.hpp"
#include "texture/drawable/EDrawable.hpp"
#include "texture/EGridMoveAnimation.hpp"
#include "resource/EResourceManager.hpp"

#include "resource/EVisualNovelScene.hpp"

EVisualNovelScene::EVisualNovelScene(std::string name)
{
	LOG_INFO("EVisualNovelScene[%s] created", name.c_str());
	this->name = name;

	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	SDL_Color bgColor = { 0x0, 0x0, 0x0 };

	std::shared_ptr<EDrawable> dr(new EDrawable(100, 100));
	auto result = _drawable_map.emplace("Talkbox", dr);
	if (!result.second) {
		LOG_ERR("Failed to insert Drawable !");
	}

	/* TODO: It would be better not to use utf-8 char directly. */
	std::shared_ptr<ETextTexture> txt(
		new ETextTexture("¾È³ç?", textColor, bgColor));
	txt->movePositionTo(10, 700);

	auto result2 = _text_texture_map.emplace("text", txt);
	if (!result2.second) {
		LOG_ERR("Failed to insert text set !");
	}

}

EVisualNovelScene::~EVisualNovelScene()
{
	LOG_INFO("EVisualNovelScene[%s] removed", name.c_str());
}

void EVisualNovelScene::testAnimation(AnimationState state)
{
	std::shared_ptr<EAnimation> ani;
	for (auto& it : _img_texture_map)
	{
		auto& sprite = it.second;
		switch (state) {
		case ANI_STOP:
			sprite->stopAnimation();
			break;
		case ANI_START:
			ani = std::shared_ptr<EAnimation>(new EGridMoveAnimation());
			sprite->setAnimation(ani);
			sprite->startAnimation();
			/* Update position animation finished? */
			break;
		case ANI_PAUSE:
			sprite->pauseAnimation();
			break;
		case ANI_RESUME:
			sprite->resumeAnimation();
			break;
		}
	}

	for (auto& it : _drawable_map)
	{
		auto& drawable = it.second;
		switch (state) {
		case ANI_STOP:
			drawable->stopAnimation();
			break;
		case ANI_START:
			ani = std::shared_ptr<EAnimation>(new EAnimation());
			drawable->setAnimation(ani);
			drawable->startAnimation();
			/* Update position animation finished? */
			break;
		case ANI_PAUSE:
			drawable->pauseAnimation();
			break;
		case ANI_RESUME:
			drawable->resumeAnimation();
			break;
		}
	}
}

void EVisualNovelScene::handleEvent(SDL_Event e)
{
	/* Handler events for Scene instance */
	bool ret = false;
	if (e.type == SDL_KEYDOWN) {
		LOG_INFO("Pressed [%d]", e.key.keysym.sym);
		switch (e.key.keysym.sym) {
		case SDLK_9:
			LOG_INFO("Play Animation");
			testAnimation(ANI_START);
			break;
		case SDLK_0:
			LOG_INFO("Stop Animation");
			testAnimation(ANI_STOP);
			break;
		case SDLK_MINUS:
			LOG_INFO("Pause Animation");
			testAnimation(ANI_PAUSE);
			break;
		case SDLK_EQUALS:
			LOG_INFO("Resume Animation");
			testAnimation(ANI_RESUME);
			break;
		}
	}
}

void EVisualNovelScene::render()
{
	for (auto &it : _img_texture_map)
	{
		it.second->render();
	}

	for (auto& it : _sprite_map)
	{
		it.second->render();
	}

	for (auto& it : _drawable_map)
	{
		it.second->render();
	}

	for (auto& it : _text_texture_map)
	{
		it.second->render();
	}
}

void EVisualNovelScene::update(Uint32 currentTime, Uint32 accumulator)
{
	if (false == isActivated)
		return;

	for (auto& it : _sprite_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for (auto &it : _img_texture_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for (auto& it : _drawable_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for (auto& it : _text_texture_map)
	{
		it.second->update(currentTime, accumulator);
	}
}
