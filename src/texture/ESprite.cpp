#include "Ecore.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResource.hpp"
#include "util/LogHelper.hpp"

#include "texture/ESprite.hpp"

/* Adjust the position on Y axis to remove mismatching with grid map */
/* TODO: Should be handled more dynamic way */
#define GRID_SIZE	(32.0f)

ESprite::ESprite(std::string _name, std::shared_ptr<ESpriteType> spriteType) :
	EDrawable()
{
	sprite_index = 0;
	sprite_change = 1;
	prevTime = 0;
	//controllable = false;

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
	_createTexture();

	if (gSpriteClips.size() > 0) {
		mWidth = gSpriteClips[0].w;
		mHeight = gSpriteClips[0].h;
	}
}

ESprite::~ESprite()
{
	_removeTexture();
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

void ESprite::_createTexture()
{
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	if (base_image_name.empty()) {
		LOG_ERR("base_image_name is empty !");
		return;
	}

	if (nullptr != mTexture) {
		LOG_ERR("Already allocated.");
		return;
	}

	/* Get texture from ResourceManager */
	sprite_base = resManager.getImageResource(base_image_name);

	if (!sprite_base)
		return;

	mTexture = sprite_base->getTexture();
}

void ESprite::_removeTexture()
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
}

void ESprite::render(int delta_x, int delta_y, double delta_angle)
{
	double ani_x = 0, ani_y = 0;

	ani_x = delta_x;
	ani_y = delta_y;

	//texture_render(200, 200, &gSpriteClips[sprite_index]);
	//texture_render_resize(100, 100, &gSpriteClips[sprite_index], 4);
	if (mTexture) {
		texture_render(
			(int)(p_x + ani_x),
			(int)(p_y + ani_y - (mHeight - GRID_SIZE)),
			&gSpriteClips[sprite_index],
			delta_angle
		);
	}
}