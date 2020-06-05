#pragma once
#include "GameObject.h"
#define BOOMERANG_ITEM_BBOX_WIDTH		15
#define BOOMERANG_ITEM_BBOX_HEIGHT		14
class BoomerangItem : public CGameObject
{
public:
	BoomerangItem();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

