#pragma once
#include "GameObject.h"
#define EFFECT_STATE_FLAME	0
#define EFFECT_STATE_100	1
#define EFFECT_STATE_400	2
#define EFFECT_STATE_700	3
#define EFFECT_STATE_2000	4
#define EFFECT_STATE_DESTROY	5
class Effect : public CGameObject
{
public:
	Effect();
	~Effect();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	DWORD timer;
};

