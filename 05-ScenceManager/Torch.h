#pragma once
#include "GameObject.h"
#include "Items.h"
#define TORCH_BBOX_WIDTH  16
#define TORCH_BBOX_HEIGHT 32

#define TORCH_DESTROYED 1
#define TORCH_DESTROYED_EFFECT_TIME 300
class Torch : public CGameObject
{
public:
	Torch();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

