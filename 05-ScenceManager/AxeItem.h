#pragma once
#include "GameObject.h"
#define AXE_ITEM_BBOX_WIDTH	14
#define	AXE_ITEM_BBOX_HEIGHT	15
class AxeItem : public CGameObject
{
public:
	AxeItem();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

