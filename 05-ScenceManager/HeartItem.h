#pragma once
#include "GameObject.h"
#define HEART_BBOX_WIDTH		12
#define HEART_BBOX_HEIGHT	10
class HeartItem : public CGameObject
{
public:
	HeartItem();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

