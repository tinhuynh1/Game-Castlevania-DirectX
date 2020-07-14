#pragma once
#include "GameObject.h"
#include "Skeleton.h"
class Bone : public CGameObject
{
public:
	float maxy;
	float firstx;
	Bone();
	~Bone();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	//RECT GetBound();
};

