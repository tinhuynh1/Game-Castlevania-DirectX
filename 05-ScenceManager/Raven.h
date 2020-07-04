#pragma once
#include "GameObject.h"
class Raven : public CGameObject
{
public:
	Raven();
	~Raven();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	RECT GetBound();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

