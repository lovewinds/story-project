#pragma once
#include "ETexture.h"
class ESDL2_Texture :
	public ETexture
{
public:
	ESDL2_Texture(Ecore* core);
	virtual ~ESDL2_Texture();
};

