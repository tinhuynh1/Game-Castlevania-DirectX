#pragma once
#include "GameObject.h"
#include "Animations.h"
#define WHIP_ANI 0
#define NORMAL_WHIP		0
#define SHORT_CHAIN		1
#define LONG_CHAIN			2
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
	bool isColliding(RECT rect_object);
	void SetWhipPosition(D3DXVECTOR2 simonPos, bool isStanding);
	void LevelUp();
	RECT GetBound();
};

