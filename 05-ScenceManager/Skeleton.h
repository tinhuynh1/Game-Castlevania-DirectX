#pragma once
#include "GameObject.h"
class Skeleton : public CGameObject
{
public:
	Skeleton();
	~Skeleton();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	RECT GetBound();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

