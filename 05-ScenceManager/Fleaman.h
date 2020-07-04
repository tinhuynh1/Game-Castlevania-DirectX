#pragma once
#include "GameObject.h"
#include "Simon.h"
#define STATE_IDLE 0
#define STATE_JUMP 1
#define FLEAMAN_GRAVITY 0.002f
class Fleaman : public CGameObject
{
public:
	Simon* mSimon;
	Fleaman();
	Fleaman(Simon* simon);
	~Fleaman();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	RECT GetBound();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

