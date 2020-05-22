#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "HeartItem.h"
#include "ChainItem.h"
#include "DaggerItem.h"
#define SIMON_WALKING_SPEED		0.025f 
#define SIMON_WALKING_TO_STAIR_SPEED	0.015f 
#define SIMON_JUMP_SPEED_Y		0.18f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			 0.0007f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING			100
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_SIT		     500
#define SIMON_STATE_ATTACK		600
#define	SIMON_STATE_SIT_AND_ATTACK 700
#define SIMON_STATE_ONSTAIR			800

#define SIMON_ANI_IDLE			0
#define SIMON_ANI_WALKING	    1
#define SIMON_ANI_JUMP			3
#define SIMON_ANI_SIT		    2
#define SIMON_ANI_ATTACK	    4
#define SIMON_ANI_SIT_AND_ATTACK	5
#define SIMON_ANI_UPSTAIR_STOP	6
#define SIMON_ANI_UPSTAIR	7
#define SIMON_ANI_DOWNSTAIR_STOP	8
#define SIMON_ANI_DOWNSTAIR	9
 

#define SIMON_ANI_DIE				8
 
#define SIMON_BBOX_WIDTH  15
#define SIMON_BBOX_HEIGHT 27

#define SIMON_UNTOUCHABLE_TIME 5000
#define SIMON_ATTACK_TIME	300

class Simon : public CGameObject
{
public:
	//static vector<LPGAMEOBJECT> listStair;
	static Simon* __instance;
	Whip* whip;
	int untouchable;
	DWORD untouchable_start;
	bool isStanding;
	int StairDirection;
	bool isWalkingToStair; //biến dùng để xác định lúc đang đi bộ đến vị trí lên cầu thang thì đi chậm
	bool isJumping = false;
	bool isLand = false;
	bool isSitAttack = false;
	bool isAttack = false;
	bool isOnStair = false;
	bool isStopOnStair = false;
	bool isUpstair = false;
	bool isHitTopStair = false;
	bool isHitBottomStair = false;

	bool isHitTop = false; //biến này dùng để ko cho Simon ngồi xuống khi ấn phím xuống cầu thang
	bool isHitLeft = false;

	float start_x;
	float start_y;
	float firstY;
public:
	
	Simon(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Reset();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	RECT GetBound();
	void CheckCollisionOnStair(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void CheckCollisionWithGround(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	
};

