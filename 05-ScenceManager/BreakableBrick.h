#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Utils.h"
#include "Simon.h"
#include "Whip.h"
#include "Rock.h"
#define BREAKABLE_BRICK_BBOX_WIDTH  16
#define BREAKABLE_BRICK_BBOX_HEIGHT 32

#define BREAKABLE_BRICK_STATE_ORIGIN	0
#define BREAKABLE_BRICK_STATE_HALF_PART 1

class BreakableBrick : public CGameObject
{
	int count = 0;
	bool isOrigin = true;
	DWORD time = 300;
public:
	Simon* simon;
	BreakableBrick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	RECT GetBound();
};

