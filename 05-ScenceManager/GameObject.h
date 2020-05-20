#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"
#include "Animations.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define ITEM_GRAVITY	0.004f
class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:
	static CGameObject* __instance;
	enum StairTypes
	{
		Null,
		ToLeft,
		ToRight
	};
	StairTypes StairTag;
	int width, height;
	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;	 

	int state;
	bool isOnGround = false;
	DWORD dt; 

	LPANIMATION_SET animation_set;
	bool visible;
	bool isCollisionWithStair = false;
	bool isMoving = false;
	int itemId;

public: 
	static CGameObject* GetInstance();
	void SetPosition(float x, float y) { this->x = x, this->y = y;}
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }

	void RenderBoundingBox();
	void SetPosition(D3DXVECTOR2 pos) { SetPosition(pos.x, pos.y); }

	D3DXVECTOR2 GetPosition() { return D3DXVECTOR2(x, y); }
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	bool AABB(RECT rect1, RECT rect2);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny);

	CGameObject();
	 
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	// Orientation
	void SetOrientation(int nx) { this->nx = nx; }
	int GetOrientation() { return nx; }
	// Visibility
	void SetVisible(bool visible) { this->visible = visible; }
	bool isVisible() { return this->visible; }
	//ground
	virtual bool IsOnGround();
	virtual void SetOnGround(bool flag);
	
	int GetItemId() { return itemId; }
	void SetItemId(int id) { this->itemId = id; }
	void SetWidth(int w) { this->width = w; }
	int GetWidth() { return this->width; }
	void SetHeight(int h) { this->height = h; }
	int GetHeight() { return this->height; }
	virtual RECT GetBound();
	~CGameObject();
};

