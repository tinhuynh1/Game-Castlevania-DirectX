#pragma once
#include "GameObject.h"
class Ghost : public CGameObject
{
public:
	Ghost();
	~Ghost();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	RECT GetBound();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

