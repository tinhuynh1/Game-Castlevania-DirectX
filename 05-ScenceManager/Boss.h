#pragma once
#include "GameObject.h"
#include "Simon.h"

#define FLY_AFTER_SLEEP 1
#define ATTACK 2
#define FLY_AFTER_ATTACK 3
#define FLY_TO_THE_MIDDLE 4
#define AIM 5
class Boss : public CGameObject
{
public:
	Boss();
	Boss(Simon* simon);
	~Boss();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	RECT GetBound();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool isDead;
protected:
	Simon* mSimon;
	int ani;
	DWORD timerFlyUp = 0;
	DWORD timerAim = 0;
	DWORD timerAttack = 0;
	bool isAttack;
	bool isHitSimon;
	bool isAtBottom;
};

