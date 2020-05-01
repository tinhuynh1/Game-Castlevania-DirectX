#pragma once
#include "GameObject.h"
#include "Animations.h"
#define WHIP_ANI 0

#define WHIP_BBOX_HEIGHT 9
#define WHIP_BBOX_WIDTH 25
class Whip : public CGameObject
{
public:
	Whip();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render(){}
	virtual void Render(int currentFrame);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float &bottom);
	void SetWhipPosition(D3DXVECTOR2 simonPos, bool isStanding);
};

