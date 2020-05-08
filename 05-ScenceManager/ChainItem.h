#pragma once
#include "GameObject.h"
#define CHAIN_BBOX_WIDTH		16
#define CHAIN_BBOX_HEIGHT		16
class ChainItem : public CGameObject
{
public:
	ChainItem();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

