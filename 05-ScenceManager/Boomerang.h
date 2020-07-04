#pragma once
#include "GameObject.h"
#include "Game.h"
#define BOOMERANG_SPEED				0.087f

#define BOOMERANG_WEAPON_BBOX_WIDTH		16
#define BOOMERANG_WEAPON_BBOX_HEIGHT		16
class Boomerang : public CGameObject
{
	float sX;
	bool isReturn = false;
	DWORD turnoverDelayTime = 0;
public:
	Boomerang();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	RECT GetBound();
	void SetDame(int dame) { this->damage = dame; }
};

