#pragma once
#include "GameObject.h"
#include "Simon.h"
#include "define.h"
class Fleaman : public CGameObject
{
public:
	Fleaman(Simon* simon);
	~Fleaman();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	RECT GetBound();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
protected:
	bool isCanReturn = false;
	bool isJumping = false;
	DWORD timerToReady = 0;
	Simon* mSimon;
};

