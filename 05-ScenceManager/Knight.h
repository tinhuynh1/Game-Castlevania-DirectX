#pragma once 
#include "GameObject.h"
#include "Simon.h"
#include "Utils.h"
#include "Brick.h"
#include "Game.h"
#define KNIGHT_STATE_WALKING	0
#define KNIGHT_STATE_DIE			1

#define KNIGHT_BBOX_WIDTH	16
#define KNIGHT_BBOX_HEIGHT	32

#define BLACK_KNIGHT_WALKING_SPEED	0.03f
#define MAX_X1 80
#define MAX_X2 114
class Knight : public CGameObject
{
	float start_x;
	float start_y;
	int count = 0;
	bool isNormal = true;
public:
	Knight(float x = 0.0f, float y = 0.0f);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	RECT GetBound();
};

