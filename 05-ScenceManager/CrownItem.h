#pragma once
#include "GameObject.h"
#include "Simon.h"

#define CROWN_BBOX_WIDTH		16
#define CROWN_BBOX_HEIGHT		16
#define CROWN_GROWING_UP_SPEED	0.01
#define MAX_Y		160
#define DISTANCE_SIMON_GROW_UP_X 220

class CrownItem : public CGameObject
{
public:
	CrownItem();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

