#include "Ecore.hpp"
#include "util/LogHelper.hpp"
#include "resource/EResourceManager.hpp"
#include "resource/EImageResource.hpp"

#include "texture/EGridMapTexture.hpp"

#define T1	16
#define T2	327

EGridMapTexture::EGridMapTexture(std::string name, std::string base_image) :
	EDrawable()
{
	this->name = name;
	this->base_image = base_image;

    /* TODO: Load map info dynamically */
	std::vector<std::vector<short> > arr({
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T2, T2, T2, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T2, T2, T2, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T2, T2, T2, T2, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T2, T2, T2, T2, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T2, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T2, T2, T1, T1, T1, T1, T2, T2, T2, T2, T2, T2, T2, T2, T1, T1, T1, T1, T2, T2, T2, T2, T1, T1, T1, T1, T2, T2, T2, T2, T2, T2, T2, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T2, T2, T1, T1, T1, T1, T2, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T2, T2, T2, T2, T1, T1, T1, T1, T2, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T2, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T2, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T2, T2, T2, T2, T2, T2, T2, T2, T2, T2, T2, T2, T2, T1, T2, T1, T1, T1, T1, T1, T2, T2, T2, T2, T2, T2, T2, T2, T2, T2, T2, T2, T2, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T2, T2, T2, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T2, T2, T2, T2, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T2, T2, T2, T2, T2, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T2, T2, T2, T2, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T2, T2, T2, T2, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 }),
		std::vector<short>({ T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1, T1, T1, T2, T1, T1, T1, T1 })
    });
    tileMap = arr;
    wTileCount = 40;
    hTileCount = 40;
}

EGridMapTexture::~EGridMapTexture()
{
	/* Deallocate */
	deallocate();
}

bool EGridMapTexture::allocate()
{
	EResourceManager& resManager = Ecore::getInstance()->getResourceManager();
	if (base_image.empty()) {
		LOG_ERR("base_image is empty !");
		return false;
	}

	tile_image = resManager.getImageResource(base_image);

	if (!tile_image)
		return false;

	if (mWidth == 0)
		mWidth = tile_image->getWidth();
	if (mHeight == 0)
		mHeight = tile_image->getHeight();

	/* Get shared texture from Resource Manager */
	mTexture = tile_image->getTexture();
	return true;
}

void EGridMapTexture::deallocate()
{
	mTexture.reset();
	if (nullptr != tile_image)
		tile_image->releaseTexture();
}

void EGridMapTexture::update(Uint32 currentTime, Uint32 accumulator)
{
}

void EGridMapTexture::render()
{
	SDL_Window* window = Ecore::getInstance()->getWindow();
	SDL_Rect rect = { 0, };
    const unsigned int base_width = 256;
    const unsigned int base_height = 1312;
    const unsigned int cell_width = 32;
    const unsigned int cell_height = 32;
	unsigned int row_count = (unsigned int)(base_width / cell_width);
	unsigned int col_count = (unsigned int)(base_height / cell_height);
    unsigned int index = 0;

    if (row_count == 0 || col_count == 0) {
		return;
    }

	if (mTexture) {
		for (int y = 0; y < hTileCount; y++) {
			for (int x = 0; x < wTileCount; x++) {
				index = (unsigned int)tileMap[y][x];
                unsigned int tx = (unsigned int)(index % row_count);
                unsigned int ty = (unsigned int)(index / row_count);

                rect.x = cell_width * tx;
                rect.y = cell_height * ty;
                rect.w = cell_width;
                rect.h = cell_height;

                texture_render((int)(x*cell_width), (int)(y*cell_height), &rect);
            }
        }
	}
}

int EGridMapTexture::getWidth()
{
	return mWidth;
}

int EGridMapTexture::getHeight()
{
	return mHeight;
}

void EGridMapTexture::setPos(int x, int y)
{
	p_x = x;
	p_y = y;
}

std::string EGridMapTexture::getName()
{
	return name;
}
