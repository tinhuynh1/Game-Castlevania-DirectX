#pragma once
#include "GameObject.h"
#include "Utils.h"
class Axe : public CGameObject
{
public:
	Axe();
	~Axe();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	RECT GetBound();
	float GetFirstPos() { return firstx; }
	float firstx;
	DWORD timer = 0;
};

