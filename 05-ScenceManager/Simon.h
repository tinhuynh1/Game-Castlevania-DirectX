#pragma once
#include "GameObject.h"
#include "Whip.h"
//#include "Dagger.h"
#include "HeartItem.h"
#include "ChainItem.h"
 #include "DaggerItem.h"
#define SIMON_WALKING_SPEED		0.06f 
#define SIMON_WALKING_TO_STAIR_SPEED	0.058f 
#define SIMON_JUMP_SPEED_Y		0.18f
#define SIMON_JUMP_DEFLECT_SPEED 0.188f
#define SIMON_GRAVITY			 0.0005f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING			100
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_SIT		     500
#define SIMON_STATE_ATTACK		600
#define	SIMON_STATE_SIT_AND_ATTACK 700
#define SIMON_STATE_ONSTAIR			800
#define SIMON_STATE_THROW	900
#define SIMON_STATE_ATTACK_UPSTAIR	1000
#define SIMON_STATE_ATTACK_DOWNSTAIR	1100
#define SIMON_STATE_DEFLECT	1200


#define SIMON_ANI_IDLE			0
#define SIMON_ANI_WALKING	    1
#define SIMON_ANI_JUMP			2
#define SIMON_ANI_SIT		    3
#define SIMON_ANI_ATTACK	    4
#define SIMON_ANI_SIT_AND_ATTACK	5
#define SIMON_ANI_UPSTAIR	6
#define SIMON_ANI_UPSTAIR_STOP	7
#define SIMON_ANI_DOWNSTAIR	8
#define SIMON_ANI_DOWNSTAIR_STOP	9
#define SIMON_ANI_ATTACK_UPSTAIR	10
#define SIMON_ANI_ATTACK_DOWNSTAIR	11
#define SIMON_ANI_CHANGECOLOR 13
#define SIMON_ANI_THROW	14
#define SIMON_ANI_DEFLECT	15
#define	SIMON_ANI_THROW_UP_STAIR	16
#define	SIMON_ANI_THROW_DOWN_STAIR	17
#define SIMON_ANI_DIE 18

#define SIMON_BBOX_WIDTH  16
#define SIMON_BBOX_HEIGHT 30

#define SIMON_UNTOUCHABLE_TIME 10000
#define SIMON_ATTACK_TIME	300
#define SIMON_DEFLECT_TIME				1000
#define SIMON_DEFLECT_SPEED_X			0.06f
#define SIMON_DEFLECT_SPEED_Y			0.14f

class Simon : public CGameObject
{
public:
	//thông tin trên HUB
	int health; //Health Simon
	int numLife;
	int numHeart;
	int idSubWeapon;
	int score;
	//static vector<LPGAMEOBJECT> listStair;
	static Simon* __instance;
	Whip* whip;
	//Dagger* dagger;
	int untouchable;
	DWORD untouchable_start;
	bool isJumping = false;
	bool isSitAttack = false;
	bool isAttack = false;
	bool isLastFrame = false;
	bool isInvisible;
	bool isEatingItem = false;
	bool isStanding;

	int StairDirection;
	bool isWalkingToStair; //biến dùng để xác định lúc đang đi bộ đến vị trí lên cầu thang thì đi chậm
	bool isOnStair = false;
	bool isStopOnStair = false;
	bool isHitTopStair = false;
	bool isHitBottomStair = false;
	bool isUpstair = false;

	bool isHitTop = false; //biến này dùng để ko cho Simon ngồi xuống khi ấn phím xuống cầu thang
	bool isHitLeft = false;

	float start_x;
	float start_y;
	float firstY;
	DWORD timerHitEnemy = 0;
	DWORD timerDie = 0;
	DWORD timerChangeColor = 0;
	bool isRevive;

	bool isUseStop = false;
	bool isOnMoving = false;

public:
	static Simon* GetInstance();
	Simon(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Reset();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	RECT GetBound();
	void CheckCollisionOnStair(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void CheckCollisionWithGround(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);

	//Get
	int GetHealth() { return health; }
	int GetNumLife() { return numLife; }
	int GetNumHeart() { return numHeart; }
	int GetSubWeapon() { return idSubWeapon; }
	int GetScore() { return score; }
	//set
	void SetNumLife(int numLife) { this->numLife = numLife; }
	void SetHealth(int health) { this->health = health; }
	void SetNumHeart(int numHeart) { this->numHeart = numHeart; }
	void SetSubWeapon(int subWeapon) { this->idSubWeapon = subWeapon; }
	void SetScore(int score) { this->score += score; }
	//calculate
	void DecreaseHealth() { health = health - 2; }
	void IncreaseNumHeart(int heartSimonNum) { this->numHeart += heartSimonNum; }
};

