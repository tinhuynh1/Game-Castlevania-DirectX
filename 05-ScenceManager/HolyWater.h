#pragma once
#include "GameObject.h"
class HolyWater
{
public:
	HolyWater();
	HolyWater(D3DXVECTOR2 pos, bool flag);
	~HolyWater();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	RECT GetBound();
	D3DXVECTOR2 firstPos;
	float Simon_x, Simon_y, gravity;
};
