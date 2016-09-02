#pragma once
#include "ETexture.hpp"

class EFigure :	public ETexture
{
public:
	EFigure();
	virtual ~EFigure();

	virtual void calculate(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void paint(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void draw();

};
