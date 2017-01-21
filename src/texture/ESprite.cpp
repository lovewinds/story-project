#include "Ecore.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResource.hpp"
#include "util/LogHelper.hpp"

#include "texture/ESprite.hpp"

ESprite::ESprite(std::string _name, std::shared_ptr<ESpriteType> spriteType) :
	EDrawable()
{
	sprite_index = 0;
	sprite_change = 1;
	prevTime = 0;

	/* TODO: This logic should be moved proper location */
	/* No blending */
	//setBlendMode(SDL_BLENDMODE_NONE);

	name = _name;
	base_image_name = spriteType->getBaseImage();
	const std::vector<SDL_Rect>& cells = spriteType->cells;

	LOG_DBG("spriteID : %s", name.c_str());
	LOG_DBG("  baseID : %s", base_image_name.c_str());

	/* Vector copy */
	gSpriteClips.assign(cells.begin(), cells.end());

	//resource = std::shared_ptr<EImageResourceInfo>(new EImageResourceInfo(name, path));
	//LOG_INFO("resource ref. count: [%lu]", resource.use_count());
	///mTexture = _texture;
	allocate();
}

ESprite::~ESprite()
{
	deallocate();
	gSpriteClips.clear();
}

std::string ESprite::getName()
{
	return name;
}

void ESprite::setIndex(unsigned int index)
{
	if (index < gSpriteClips.size()) {
		sprite_index = index;
		LOG_DBG("Sprite[%p] index set [%d]", this, sprite_index);
	}
}

bool ESprite::allocate()
{
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	if (base_image_name.empty()) {
		LOG_ERR("base_image_name is empty !");
		return false;
	}

	if (nullptr != mTexture) {
		LOG_DBG("Already allocated.");
		return false;
	}

	/* TODO: Allocate a new texture for each sprite */
	sprite_base = resManager.getImageResource(base_image_name);

	if (!sprite_base)
		return false;

	mTexture = sprite_base->getTexture();
	return true;
}

void ESprite::deallocate()
{
	mTexture.reset();
	sprite_base->releaseTexture();
}

void ESprite::update(Uint32 currentTime, Uint32 accumulator)
{
	int tick = 100;

	if (gSpriteClips.size() == 4)
		tick = 1000;

	if (currentTime - prevTime > tick)
	{
		prevTime = currentTime;

		sprite_index++;
		if (sprite_index >= (int8_t)gSpriteClips.size())
			sprite_index = 0;
	}

	/* Animation */
	if (animation) {
		animation->update(currentTime, accumulator);
	}
}

void ESprite::render()
{
	double ani_x = 0, ani_y = 0;

	if (animation) {
		ani_x = animation->getX();
		ani_y = animation->getY();
	}
	//texture_render(200, 200, &gSpriteClips[sprite_index]);
	//texture_render_resize(100, 100, &gSpriteClips[sprite_index], 4);
	if (mTexture) {
		texture_render(
			(int)(p_x + ani_x),
			(int)(p_y + ani_y),
			&gSpriteClips[sprite_index]
		);
	}
}