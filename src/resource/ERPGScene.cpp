#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "texture/ESprite.hpp"
#include "texture/EGridMoveAnimation.hpp"

#include "resource/ERPGScene.hpp"

ERPGScene::ERPGScene(std::string name)
{
	LOG_INFO("ERPGScene[%s] created", name.c_str());
	this->name = name;
#if 0
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	SDL_Color bgColor = { 0x0, 0x0, 0x0 };
	std::shared_ptr<ETextTexture> tt(new ETextTexture("test", textColor, bgColor));
	tt->movePositionTo(10, 100);
	auto result = _text_texture_map.emplace("test string", tt);
	if (!result.second) {
		LOG_ERR("Failed to insert text set !");
	}
#endif
}

ERPGScene::~ERPGScene()
{
	LOG_INFO("ERPGScene[%s] removed", name.c_str());
}

void ERPGScene::testAnimation(AnimationState state)
{
	std::shared_ptr<EAnimation> ani;
	for (auto& it : _sprite_map)
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
}

void ani_finished(void *caller, double delta_x, double delta_y)
{
	ERPGScene *scene = static_cast<ERPGScene*>(caller);

	if (nullptr == scene)
		return;


}

void ERPGScene::handleMove(GridMoveDir dir)
{
	std::shared_ptr<EAnimation> ani;
	EGridMoveAnimation* grid = nullptr;
	for (auto& it : _sprite_map)
	{
		auto& sprite = it.second;
		switch (sprite->getAnimationState())
		{
		case ANI_NONE:
			ani = std::shared_ptr<EAnimation>(new EGridMoveAnimation(dir));
			ani->setCaller(sprite);
			sprite->setAnimation(ani);
			sprite->startAnimation();
			break;
		case ANI_STOP:
			ani = sprite->getAnimation();
			grid = dynamic_cast<EGridMoveAnimation*>(ani.get());
			if (grid) {
				grid->setDirection(dir);
			}
			sprite->startAnimation();
			break;
		default:
			break;
		}
	}
}

void ERPGScene::handleEvent(SDL_Event e)
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

		case SDLK_UP:
			LOG_INFO("Move : UP");
			handleMove(DIR_UP);
			break;
		case SDLK_DOWN:
			LOG_INFO("Move : DOWN");
			handleMove(DIR_DOWN);
			break;
		case SDLK_LEFT:
			LOG_INFO("Move : LEFT");
			handleMove(DIR_LEFT);
			break;
		case SDLK_RIGHT:
			LOG_INFO("Move : RIGHT");
			handleMove(DIR_RIGHT);
			break;
		}
	}
}

void ERPGScene::render()
{
	for (auto &it : _img_texture_map)
	{
		it.second->render();
	}

	for (auto& it : _sprite_map)
	{
		it.second->render();
	}

	for (auto& it : _text_texture_map)
	{
		it.second->render();
	}

	for (auto& it : _drawable_map)
	{
		it.second->render();
	}
}

void ERPGScene::update(Uint32 currentTime, Uint32 accumulator)
{
	if (false == isActivated)
		return;

	for (auto& it : _sprite_map)
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
}
