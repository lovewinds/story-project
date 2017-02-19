#pragma once

#include <string>
#include <cmath>

#include "EDrawable.hpp"

class EImageResource;

class EImageTexture : public EDrawable
{
public:
	/* Initializes variables */
	EImageTexture(std::string name, std::string base_image);

	/* Deallocates memory */
	~EImageTexture();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);
	//virtual void paint(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void render(int delta_x, int delta_y);

	/* Gets image dimensions */
	int getWidth();
	int getHeight();
	void setWidth(double width, bool ratio = false);
	void setHeight(double height, bool ratio = false);
	void setPos(int x, int y);
	std::string getName();

protected:
	void _createTexture();
	void _removeTexture();

	int radian;
	double wRatio;
	double hRatio;

	/* Angle of rotation */
	double m_degrees;

	/* animation */
	bool animating;

	std::string name;
	std::string base_image;

	std::shared_ptr<EImageResource>	image;
};
