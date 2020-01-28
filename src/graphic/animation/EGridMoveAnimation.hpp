#pragma once

#include "graphic/animation/EAnimation.hpp"

namespace story {
namespace Graphic {

class EGridMoveAnimation : public EAnimation
{
public:
	EGridMoveAnimation();
	virtual ~EGridMoveAnimation();

	virtual void start();

	virtual void setAxisFactor(float axis_x, float axis_y);
	virtual float getAxisFactorX();
	virtual float getAxisFactorY();

	virtual void update(Uint32 currentTime, Uint32 accumulator = 0);

protected:
	double accel = 0.026;
	double velo = 10.0;
	float next_dir_factor_x = 0.0f;
	float next_dir_factor_y = 0.0f;
	float curr_dir_factor_x = 0.0f;
	float curr_dir_factor_y = 0.0f;
};

} /* namespace Graphic */
} /* namespace story */
