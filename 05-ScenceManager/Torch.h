#pragma once
#include "GameObject.h"
#define TORCH_BBOX_WIDTH  16
#define TORCH_BBOX_HEIGHT 32
class Torch : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

