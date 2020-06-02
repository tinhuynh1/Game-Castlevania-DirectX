#pragma once
#include "GameObject.h"
#include "Torch.h"
#define DAGGER_SPEED				0.087f

#define DAGGER_WEAPON_BBOX_WIDTH		16
#define DAGGER_WEAPON_BBOX_HEIGHT		10
class Dagger : public CGameObject
{
public:
	Dagger();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	RECT GetBound();
};

