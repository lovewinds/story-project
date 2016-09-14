#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "texture/ESprite.hpp"

#include "resource/ESceneInfo.hpp"

EScene::EScene(std::string name)
: sprite_map(_sprite_map)
{
	LOG_INFO("ESceneInfo[%s] created", name.c_str());
	this->name = name;

	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	SDL_Color bgColor = { 0x0, 0x0, 0x0 };
	std::shared_ptr<ETextTexture> tt(new ETextTexture("test", textColor, bgColor));
	tt->movePositionTo(10, 100);
	auto result = _text_texture_map.emplace("test string", tt);
	if (!result.second) {
		LOG_ERR("Failed to insert text set !");
	}
}

EScene::~EScene()
{
	_sprite_map.clear();
	_text_texture_map.clear();

	isAllocated = false;
	isActivated = false;

	LOG_INFO("ESceneInfo[%s] removed", name.c_str());
}

std::string EScene::getName()
{
	return name;
}

bool EScene::addSprite(std::shared_ptr<ESprite> sprite)
{
#if 0
	LOG_DBG("Trying to create sprite type [%s] / name [%s]", type.c_str(), name.c_str());

	/* TODO: Move allocation logic into Resource Manager */
	//std::shared_ptr<ESprite> sprite(new ESprite(name, base_image));
	EResourceManager& resMgr = Ecore::getInstance()->getResourceManager();
	std::shared_ptr<ESprite> sprite = resMgr.createSprite(type, name);

	//std::pair<std::map<std::string, std::shared_ptr<ESceneInfo>>::iterator, bool> result;
	auto result = _sprite_map.emplace(name, sprite);

	if (!result.second) {
		LOG_ERR("Failed to insert sprite into set !");
		return nullptr;
	}
#if 1
	LOG_INFO("Current sprites:");
	for(auto& it : _sprite_map)
	{
		//auto t = it.second.get();
		LOG_INFO("   %s", it.first.c_str());
	}
#endif
	return sprite;
#endif
	auto result = _sprite_map.emplace(sprite->getName(), sprite);
	if (!result.second) {
		LOG_ERR("Failed to insert sprite set !");
		return false;
	}
	return true;
}

bool EScene::addImage(std::shared_ptr<EImageTexture> imgTexture)
{
	auto result = _img_texture_map.emplace(imgTexture->getName(), imgTexture);
	if (!result.second) {
		LOG_ERR("Failed to insert imgTexture set !");
		return false;
	}
	return true;
}

bool EScene::allocateSprites()
{
	if (_sprite_map.empty())
		return false;

	for(auto &it : _sprite_map) {
		LOG_DBG("  Allocate sprite [%s] count [%lu]", it.first.c_str(), it.second.use_count());
		it.second->allocate();
	}
	return true;
}

bool EScene::allocateImages()
{
	if (_img_texture_map.empty())
		return false;

	for (auto &it : _img_texture_map) {
		LOG_DBG("  Allocate image texture [%s] count [%lu]", it.first.c_str(), it.second.use_count());
		it.second->allocate();
	}
	return true;
}

bool EScene::allocateTexts()
{
	if (_text_texture_map.empty())
		return false;

	for (auto &it : _text_texture_map) {
		LOG_DBG("  Allocate text texture [%s] count [%lu]", it.first.c_str(), it.second.use_count());
		it.second->allocate();
	}
	return true;
}

bool EScene::allocate()
{
	bool res = false;
	res |= allocateSprites();
	res |= allocateImages();
	res |= allocateTexts();

	if (false == res) {
		LOG_ERR("Failed to allocate !");
		deallocate();
		return false;
	}

	isAllocated = true;
	isActivated = true;

	return true;
}

void EScene::deallocate()
{
	for(auto &it : _sprite_map) {
		LOG_DBG("  DeAllocate sprite [%s] count [%lu]", it.first.c_str(), it.second.use_count());
		it.second->deallocate();
	}
	for(auto &it : _img_texture_map) {
		LOG_DBG("  DeAllocate image texture [%s] count [%lu]", it.first.c_str(), it.second.use_count());
		it.second->deallocate();
	}
	for (auto &it : _text_texture_map) {
		LOG_DBG("  DeAllocate text texture [%s] count [%lu]", it.first.c_str(), it.second.use_count());
		it.second->deallocate();
	}

	isAllocated = false;
	isActivated = false;
}

void EScene::setActiveState(bool active)
{
	isActivated = active;
}

void EScene::testAnimation(AnimationState state)
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
				ani = std::shared_ptr<EAnimation>(new EAnimation());
				sprite->setAnimation(ani);
				sprite->startAnimation();
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

void EScene::handleEvent(SDL_Event e)
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

void EScene::render()
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
#if 0
	std::list<EDrawable*>::iterator	iter = textureList.begin();
	Uint32 current = SDL_GetTicks();

	/* TODO: Handle this texture same with others */
	background->render();

	while (iter != textureList.end()) {
		EDrawable* texture = *iter;
		if (texture != NULL) {
			texture->render();
			//LOG_INFO("List Texture(%p) rendered !", texture);
		} else {
			LOG_INFO("List Texture is NULL. count: %lu", textureList.size());
		}
		iter++;
	}
	textTexture->render();
	sprite->render();
#endif
}

void EScene::update(Uint32 currentTime, Uint32 accumulator)
{
	if (false == isActivated)
		return;

	for(auto& it : _sprite_map)
	{
		it.second->update(currentTime, accumulator);
	}

	for(auto& it : _text_texture_map)
	{
		it.second->update(currentTime, accumulator);
	}
#if 0
	//std::list<ETexture*>::iterator	iter = textureList.begin();
	std::list<EDrawable*>::iterator	iter = textureList.begin();
	while (iter != textureList.end()) {
		EDrawable* texture = *iter;
		if (texture != NULL) {
			//texture->calculate(currentTime, accumulator);
			texture->update(currentTime, accumulator);
		}
		iter++;
	}

	/* TODO: Handle this texture same with others */
	background->update(currentTime, accumulator);

	textTexture->update(currentTime, accumulator);
	sprite->update(currentTime, accumulator);

	/* TEST */
	if (testState & DIR_UP)
		temp_moveCharacter(0.0, -1.0);
	if (testState & DIR_DOWN)
		temp_moveCharacter(0.0, 1.0);
	if (testState & DIR_LEFT)
		temp_moveCharacter(-1.0, 0.0);
	if (testState & DIR_RIGHT)
		temp_moveCharacter(1.0, 0.0);

	if (testBackgroundState & DIR_UP)
		temp_moveBackGround(0.0, -1.0);
	if (testBackgroundState & DIR_DOWN)
		temp_moveBackGround(0.0, 1.0);
	if (testBackgroundState & DIR_LEFT)
		temp_moveBackGround(-1.0, 0.0);
	if (testBackgroundState & DIR_RIGHT)
		temp_moveBackGround(1.0, 0.0);
#endif
}
