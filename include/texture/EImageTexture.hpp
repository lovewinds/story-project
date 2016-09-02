#pragma once

#include <string>
#include <cmath>

#include "ETexture.hpp"

class EImageResourceInfo;

class EImageTexture : public ETexture
{
public:
	/* Initializes variables */
	EImageTexture(std::string name, std::string base_image);
	EImageTexture(int x, int y);

	/* Deallocates memory */
	~EImageTexture();

	bool allocate();
	void deallocate();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	//virtual void paint(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render();

	/* Gets image dimensions */
	int getWidth();
	int getHeight();
	std::string getName();

protected:
	int radian;

	/* Angle of rotation */
	double m_degrees;

	/* animation */
	bool animating;

	std::string name;
	std::string base_image;

	std::shared_ptr<EImageResourceInfo>	image;
};
