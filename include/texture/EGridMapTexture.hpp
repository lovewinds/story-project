#pragma once

#include <string>
#include <cmath>
#include <vector>

#include "EDrawable.hpp"

class EImageResource;

class EGridMapTexture : public EDrawable
{
public:
	/* Initializes variables */
	EGridMapTexture(std::string name, std::string base_image,
			std::shared_ptr<EGridDesc> desc);
	~EGridMapTexture();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render(int delta_x, int delta_y, double delta_angle);

	/* Gets image dimensions */
	int getWidth();
	int getHeight();
	void setPos(int x, int y);
	std::string getName();

protected:
	void _createTexture();
	void _removeTexture();

	uint wTileCount;
    uint hTileCount;
	uint levels;

    std::string name;
	std::string base_image;

    std::vector< std::vector< std::vector<ushort> > > tileMap;

	std::shared_ptr<EImageResource>	tile_image;
};
