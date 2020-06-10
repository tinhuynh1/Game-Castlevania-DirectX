#pragma once
#include "GameObject.h"
#include "Simon.h"

#define CROWN_BBOX_WIDTH		16
#define CROWN_BBOX_HEIGHT		16

#define CROWN_SPEED_Y 0,12f
class CrownItem : public CGameObject
{
	bool isOn = false;
public:
	CrownItem();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

