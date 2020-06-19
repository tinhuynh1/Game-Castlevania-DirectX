#pragma once
#include "GameObject.h"
#include "Brick.h"
#define ITEM_SMALL_HEART_BBOX_WIDTH	8
#define ITEM_SMALL_HEART_BBOX_HEIGHT	8

class SmallHeartItem : public CGameObject
{
private:
	float   vx_variability;
public:
	  SmallHeartItem();
	  void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	  void Render();
	  void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

