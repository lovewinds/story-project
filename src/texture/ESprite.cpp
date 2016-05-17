#include "Ecore.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResourceInfo.hpp"
#include "util/LogHelper.hpp"

#include "texture/ESprite.hpp"

ESprite::ESprite(std::string _name, std::string _base_image, std::vector<SDL_Rect>& cells) :
	ETexture()
{
	sprite_index = 0;
	sprite_change = 1;
	prevTime = 0;

	/* TODO: This logic should be moved proper location */
	/* No blending */
	//setBlendMode(SDL_BLENDMODE_NONE);

	name = _name;
	base_image_name = _base_image;
	LOG_DBG("spriteID : %s", name.c_str());
	LOG_DBG("baseID   : %s", base_image_name.c_str());

	/* Vector copy */
	gSpriteClips.assign(cells.begin(), cells.end());

	//resource = std::shared_ptr<EImageResourceInfo>(new EImageResourceInfo(name, path));
	//LOG_INFO("resource ref. count: [%lu]", resource.use_count());
	///mTexture = _texture;
}

ESprite::~ESprite()
{
	deallocate();
}

std::string ESprite::getName()
{
	return name;
}

bool ESprite::allocate()
{
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	if (base_image_name.empty()) {
		LOG_ERR("base_image_name is empty !");
		return false;
	}

	sprite_base = resManager.getImageResource(base_image_name);

	if (!sprite_base)
		return false;

	mTexture = sprite_base->getTexture();
	return true;
}

void ESprite::deallocate()
{
	mTexture.reset();
	gSpriteClips.clear();
}

void ESprite::update(Uint32 currentTime, Uint32 accumulator)
{
	if (currentTime - prevTime > 100)
	{
		prevTime = currentTime;

		sprite_index++;
		if (sprite_index >= gSpriteClips.size())
			sprite_index = 0;
	}
}

void ESprite::render()
{
	//texture_render(200, 200, &gSpriteClips[sprite_index]);
	//texture_render_resize(100, 100, &gSpriteClips[sprite_index], 4);
	if (mTexture)
		texture_render((int)p_x, (int)p_y, &gSpriteClips[sprite_index]);
}