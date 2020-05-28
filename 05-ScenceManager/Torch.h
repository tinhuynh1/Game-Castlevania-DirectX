#pragma once
#include "GameObject.h"
#include "Items.h"


#define TORCH_STATE_BIG		0
#define TORCH_STATE_SMALL	1
#define TORCH_DESTROYED		2
#define TORCH_DESTROYED_EFFECT_TIME 300

#define TORCH_BBOX_WIDTH  16
#define TORCH_BBOX_HEIGHT 32
#define TORCH_SMALL_BBOX_WIDTH	8
#define TORCH_SMALL_BBOX_HEIGHT	16
class Torch : public CGameObject
{
public:
	Torch();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	RECT GetBound();
};

