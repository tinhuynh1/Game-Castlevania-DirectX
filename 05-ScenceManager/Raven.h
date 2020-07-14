#pragma once
#include "GameObject.h"
#include "Simon.h"
#define RAVEN_STATE_READY	0
#define RAVEN_STATE_FLY_HORIZENTAL 1
#define RAVEN_STATE_FLY_CROSS 2
#define RAVEN_STATE_FLY_AIM  3
class Raven : public CGameObject
{
public:
	DWORD timerToAim = 0;
	DWORD timerToCros = 0;
	Raven();
	~Raven();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	RECT GetBound();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

