#pragma once 
#include "GameObject.h"
#include "Simon.h"
#include "Utils.h"
#define KNIGHT_STATE_WALKING	0
#define KNIGHT_STATE_DIE			1

#define KNIGHT_BBOX_WIDTH	16
#define KNIGHT_BBOX_HEIGHT	32

#define BLACK_KNIGHT_WALKING_SPEED	0.04f
class Knight : public CGameObject
{
public:
	Knight();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	RECT GetBound();

};

