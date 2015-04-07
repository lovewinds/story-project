#pragma once
#include "ETexture.h"

class EFigure :	public ETexture
{
public:
	EFigure(Ecore* core);
	virtual ~EFigure();

	virtual void calculate(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void paint(Uint32 currentTime, Uint32 accumulator = 0);
	virtual void draw();

};
