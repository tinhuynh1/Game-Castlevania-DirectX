#pragma once
#include "GameObject.h"
#include "define.h"
#include "Simon.h"
#define SKELETON_STATE_MOVE	0
#define SKELETON_STATE_JUMP	1
class Skeleton : public CGameObject
{
public:
	Skeleton(Simon* simon);
	~Skeleton();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	RECT GetBound();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

protected:
	Simon* mSimon;
};

