#pragma once
#include "GameObject.h"
#define WATCH_ITEM_BBOX_WIDTH	15
#define	WATCH_ITEM_BBOX_HEIGHT	16
class WatchItem : public CGameObject
{
public:
	WatchItem();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

