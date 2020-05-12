#pragma once
#include "GameObject.h"
#define BOTSTAIR_BBOX_WIDTH  8
#define BOTSTAIR_BBOX_HEIGHT 8
class BotStair : public CGameObject
{
public:
	BotStair();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

