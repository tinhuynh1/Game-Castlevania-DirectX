#pragma once
#include "GameObject.h"

#define DAGGER_BBOX_WIDTH	16
#define	DAGGER_BBOX_HEIGHT	10
class DaggerItem : public CGameObject
{
public:
	DaggerItem();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

