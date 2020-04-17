#pragma once
#include "GameObject.h"
#define SIMON_WALKING_SPEED		0.08f 
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING			100
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_ANI_IDLE			0
#define SIMON_ANI_WALKING	1
#define SIMON_ANI_JUMP			2
#define SIMON_ANI_SIT				3
 

#define SIMON_ANI_DIE				8

#define	SIMON_LEVEL_SMALL	2
 

#define SIMON_BBOX_WIDTH  15
#define SIMON_BBOX_HEIGHT 27

#define SIMON_UNTOUCHABLE_TIME 5000

class Simon : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;
	float start_y;
public:
	Simon(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = 1; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Reset();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

