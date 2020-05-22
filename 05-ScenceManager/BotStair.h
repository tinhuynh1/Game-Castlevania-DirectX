#pragma once
#include "GameObject.h"
#define BOTSTAIR_BBOX_WIDTH  14
#define BOTSTAIR_BBOX_HEIGHT 15
class BotStair : public CGameObject
{
public:
	BotStair();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

