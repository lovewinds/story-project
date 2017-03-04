#include <iostream>
#include <limits>

#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "texture/ESprite.hpp"
#include "texture/EGridMoveAnimation.hpp"

#include "resource/ERPGScene.hpp"

ERPGScene::ERPGScene(std::string name)
 : EScene(name)
{
	LOG_INFO("ERPGScene[%s] created", name.c_str());
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
	/* Create base map textures */
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();

	std::shared_ptr<EGridMapTexture> map(new EGridMapTexture("MyMap", "MapTile"));
	gridMap = map;
	auto result = _raw_texture_map.emplace("GridMap", map);
	if (!result.second) {
		LOG_ERR("Failed to insert texture!");
	}

	/* Create sprite by manually for test */
	std::shared_ptr<story::Graphic::Object> object(new story::Graphic::Object());
	std::shared_ptr<ESprite> sprite;

	sprite = resManager.createSprite("char_girl", "movingChar");
	object->setName(sprite->getName());
	object->movePositionTo(10, 10);
	object->addSprite(sprite);
	
	auto ins = _object_map.emplace("movingChar", object);
	if (!ins.second) {
		LOG_ERR("Failed to insert object map!");
	}
}

ERPGScene::~ERPGScene()
{
	gridMap.reset();

	LOG_INFO("ERPGScene[%s] removed", name.c_str());
}

/* Store Image and Sprite to render position based */
bool ERPGScene::addObject(std::shared_ptr<story::Graphic::Object> object)
{
	double _x = 0.0;
	double _y = 0.0;
	std::ostringstream stringStream;
	std::string position_str;
	unsigned long pos = 0;

	LOG_DBG("Check object position [%3d, %3d]",
		(int)object->getPositionX(), (int)object->getPositionY());

	_x = object->getPositionX() * 32.0;
	_y = object->getPositionY() * 32.0;
	object->movePositionTo(_x, _y);

	/* Make position based string
	 *    YYYYXXXX
	 */
	pos = ((unsigned int)_y & 0xFFFF) << 16;
	pos = pos + ((unsigned int)_x & 0xFFFF);
  	stringStream << std::hex << std::setfill('0') << std::setw(8)
  		<< pos << std::dec
  		<< "_" << object->getName();
	position_str = stringStream.str();
	LOG_DBG("POS [%s]", position_str.c_str());

	/* Convert into actual position from grid position */
	auto result = _object_map.emplace(position_str, object);
	if (!result.second) {
		LOG_ERR("Failed to insert object map!");
		return false;
	}
	return true;
}

void ERPGScene::testAnimation(AnimationState state)
{
	std::shared_ptr<EAnimation> ani;
	//for (auto& it : _sprite_map)
	for (auto& it : _object_map)
	{
		auto& object = it.second;
		switch (state) {
		case ANI_STOP:
			object->stopAnimation();
			break;
		case ANI_START:
			ani = std::shared_ptr<EAnimation>(new EGridMoveAnimation());
			object->setAnimation(ani);
			object->startAnimation();
			/* Update position animation finished? */
			break;
		case ANI_PAUSE:
			object->pauseAnimation();
			break;
		case ANI_RESUME:
			object->resumeAnimation();
			break;
		default:
			break;
		}
	}
}

void ERPGScene::handleDirectonFactor(float axis_x, float axis_y)
{
	std::shared_ptr<EAnimation> ani;
	EGridMoveAnimation* grid = nullptr;

	for (auto& it : _object_map)
	{
		if (it.second->isControllable()) {
			auto& object = it.second;
			switch (object->getAnimationState())
			{
			case ANI_NONE:
				LOG_DBG("Start animation. [%f / %f]", axis_x, axis_y);
				ani = std::shared_ptr<EAnimation>(new EGridMoveAnimation());
				ani->setCaller(object);
				grid = dynamic_cast<EGridMoveAnimation*>(ani.get());
				grid->setAxisFactor(axis_x, axis_y);

				object->setAnimation(ani);
				object->startAnimation();
			break;
			case ANI_START:
				/* Already in progress, Just update axis factor */
				ani = object->getAnimation();
				grid = dynamic_cast<EGridMoveAnimation*>(ani.get());
				if (grid) {
					grid->setAxisFactor(axis_x, axis_y);
				}
			break;
			default:
				/* Already in progress */
				ani = object->getAnimation();
				grid = dynamic_cast<EGridMoveAnimation*>(ani.get());
				if (grid) {
					grid->setAxisFactor(axis_x, axis_y);
				}
				object->startAnimation();
			break;
			}
		}
	}
}

/* TODO: Logic should be changed to register clickable position
   for each object */
bool ERPGScene::testRotate(int x, int y)
{
	bool event_consume = false;

	//for (auto& it : _sprite_map)
	for (auto& it : _object_map)
	{
		std::shared_ptr<story::Graphic::Object> object = it.second;
		if (nullptr == object) continue;

		int obj_x = object->getPositionX();
		int obj_y = object->getPositionY();
		int obj_x2 = object->getPositionX() + object->getWidth();
		int obj_y2 = object->getPositionY() + object->getHeight();

		if (obj_x <= x && x <= obj_x2 &&
			obj_y <= y && y <= obj_y2)
		{
			LOG_INFO("Rotate [%s]", object->getName().c_str());
			object->changeRotateState(object);

			event_consume = true;
			return true;
		}
	}

	return false;
}

void ERPGScene::handleEvent(SDL_Event e)
{
	/* Handler events for Scene instance */
	bool ret = false;
	if (e.type == SDL_KEYDOWN) {
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
			//LOG_INFO("Move : UP");
			//handleMove(DIR_UP);
			handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), -1.0f);
			break;
		case SDLK_DOWN:
			//LOG_INFO("Move : DOWN");
			//handleMove(DIR_DOWN);
			handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 1.0f);
			break;
		case SDLK_LEFT:
			//LOG_INFO("Move : LEFT");
			//handleMove(DIR_LEFT);
			handleDirectonFactor(-1.0f, std::numeric_limits<float>::quiet_NaN());
			break;
		case SDLK_RIGHT:
			//LOG_INFO("Move : RIGHT");
			//handleMove(DIR_RIGHT);
			handleDirectonFactor(1.0f, std::numeric_limits<float>::quiet_NaN());
			break;
		case SDLK_SPACE:
			LOG_INFO("Animation test");
			std::shared_ptr<story::Graphic::Object> found;
			auto search = _object_map.find("movingChar");
			if (search != _object_map.end()) {
				found = search->second;
				if (found->getAnimationState() != ANI_START)
					found->changeState(found);
				LOG_DBG("Finished");
			} else {
				LOG_ERR("Object was not found !");
			}
			break;
		}
	}
	else if (e.type == SDL_KEYUP) {
		switch (e.key.keysym.sym) {
		case SDLK_UP:
			LOG_INFO("Key released : [UP]");
			handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 0.0f);
		break;
		case SDLK_DOWN:
			LOG_INFO("Key released : [DOWN]");
			handleDirectonFactor(std::numeric_limits<float>::quiet_NaN(), 0.0f);
		break;
		case SDLK_LEFT:
			LOG_INFO("Key released : [LEFT]");
			handleDirectonFactor(0.0f, std::numeric_limits<float>::quiet_NaN());
		break;
		case SDLK_RIGHT:
			LOG_INFO("Key released : [RIGHT]");
			handleDirectonFactor(0.0f, std::numeric_limits<float>::quiet_NaN());
		break;
		default: break;
		}
	}
	else if (e.type == SDL_FINGERDOWN) {
		SDL_TouchFingerEvent *te = &e.tfinger;
		int x = 0, y = 0;
		LOG_INFO("Handle event! type: SDL_FINGERDOWN / %u", te->timestamp);

		x = Ecore::getScreenWidth() * te->x;
		y = Ecore::getScreenHeight() * te->y;
		LOG_INFO("Touched [%d, %d]  | [%03d, %03d]", x, y,
				Ecore::getScreenWidth(), Ecore::getScreenHeight());

		/* TODO: event consumer should be handle more efficient way */
		bool consumed = testRotate(x, y);
		if (consumed)
			return;

		std::shared_ptr<story::Graphic::Object> found;
		auto search = _object_map.find("movingChar");
		if (search != _object_map.end()) {
			found = search->second;
			if (found->getAnimationState() != ANI_START)
				found->animatedMoveTo(found, x, y, 1000);
			LOG_DBG("Finished");
		} else {
			LOG_ERR("Object was not found !");
		}
	} else if (e.type == SDL_MOUSEBUTTONDOWN) {
		LOG_INFO("Animation test [%03d, %03d]", e.button.x, e.button.y);

		/* TODO: event consumer should be handle more efficient way */
		bool consumed = testRotate(e.button.x, e.button.y);
		if (consumed)
			return;

		/* Move to clicked position */
		std::shared_ptr<story::Graphic::Object> found;
		auto search = _object_map.find("movingChar");
		if (search != _object_map.end()) {
			found = search->second;
			if (found->getAnimationState() != ANI_START)
				found->animatedMoveTo(found, e.button.x, e.button.y, 1000);
			LOG_DBG("Finished");
		} else {
			LOG_ERR("Object was not found !");
		}
	} else if (e.type == SDL_FINGERMOTION) {
#if 0
		SDL_TouchFingerEvent *te = &e.tfinger;
		int ax = (int)(((te->dx > 0.0) ? te->dx : te->dx * -1.0) * 1000);
		int ay = (int)(((te->dy > 0.0) ? te->dy : te->dy * -1.0) * 1000);

		LOG_INFO("Handle event! type: SDL_FINGERMOTION");
		LOG_INFO("dx / dy : [%f / %f]", te->dx, te->dy);
		LOG_INFO("ax / ay : [%d / %d]", ax, ay);

		if (te->x <= 0.3) {
			LOG_INFO("Move : LEFT");
			handleMove(DIR_LEFT);
		}
		else if (te->x >= 0.6) {
			LOG_INFO("Move : RIGHT");
			handleMove(DIR_RIGHT);
		}
#endif
	}
}

void ERPGScene::render()
{
	gridMap->render(0, 0, 0);

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

	for (auto& it : _object_map)
	{
		it.second->update(currentTime, accumulator);
	}

	gridMap->update(currentTime, accumulator);
}
