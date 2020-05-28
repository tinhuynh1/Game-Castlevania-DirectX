#pragma once
#include "GameObject.h"
#define TOPSTAIR_BBOX_WIDTH  10
#define TOPSTAIR_BBOX_HEIGHT 10
class TopStair : public CGameObject
{
public:
	TopStair();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

