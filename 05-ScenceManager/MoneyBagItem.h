#pragma once
#include"GameObject.h"
#include "Brick.h"
#define ITEM_MONEY_BAG_BBOX_WIDTH		15
#define ITEM_MONEY_BAG_BBOX_HEIGHT	15
class MoneyBagItem : public CGameObject
{
public:
	MoneyBagItem();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

