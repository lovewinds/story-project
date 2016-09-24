#pragma once

#include <string>
#include <cmath>
#include <vector>

#include "ETexture.hpp"

class EImageResource;

class EGridMapTexture : public ETexture
{
public:
	/* Initializes variables */
	EGridMapTexture(std::string name, std::string base_image);
	~EGridMapTexture();

	virtual bool allocate();
	virtual void deallocate();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

	/* Gets image dimensions */
	int getWidth();
	int getHeight();
	void setPos(int x, int y);
	std::string getName();

protected:
	Uint32 wTileCount;
    Uint32 hTileCount;

    std::string name;
	std::string base_image;

    std::vector<std::vector<short> > tileMap;

	std::shared_ptr<EImageResource>	tile_image;
};
