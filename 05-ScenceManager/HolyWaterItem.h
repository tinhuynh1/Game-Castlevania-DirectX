#pragma once
#include "GameObject.h"
#define HOLYWATER_ITEM_BBOX_WIDTH	16
#define	HOLYWATER_ITEM_BBOX_HEIGHT	16
class HolyWaterItem : public CGameObject
{
public:
	HolyWaterItem();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

