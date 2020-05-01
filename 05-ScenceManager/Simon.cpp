#include "Simon.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"
#include "Portal.h"
#include "Torch.h"
#include "Brick.h"


Simon::Simon(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(SIMON_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	whip = new Whip();
}
void Simon::Render()
{
	int ani = -1;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else if (state == SIMON_STATE_SIT) ani = SIMON_ANI_SIT;
	else if (state == SIMON_STATE_JUMP) ani = SIMON_ANI_JUMP;
	else if (state == SIMON_STATE_ATTACK) ani = SIMON_ANI_ATTACK;
	else if (state == SIMON_STATE_SIT_AND_ATTACK) ani = SIMON_ANI_SIT_AND_ATTACK;
	else
		{
			if (vx == 0)
			{
				ani = SIMON_ANI_IDLE;
			}
			else ani = SIMON_ANI_WALKING;
		 
		}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y,nx, alpha);

	//RenderBoundingBox();
	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_AND_ATTACK)
		whip->Render(animation_set->at(ani)->GetCurrentFrame());
}
void Simon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING:
			if (nx > 0)
			{
				vx = SIMON_WALKING_SPEED;
			}
			else
			{
				vx = -SIMON_WALKING_SPEED;
			}
			break;
	case SIMON_STATE_SIT:
	{
		isStanding = false;
		vx = 0;
		vy = 0;
		break;
	}
	case SIMON_STATE_JUMP:
	{
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		isStanding = true;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	}
	case SIMON_STATE_ATTACK:
	{
		animation_set->at(SIMON_ANI_ATTACK)->Reset();
		animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());
		break;
	}
	case SIMON_STATE_SIT_AND_ATTACK:
	{
		animation_set->at(SIMON_ANI_SIT_AND_ATTACK)->Reset();
		animation_set->at(SIMON_ANI_SIT_AND_ATTACK)->SetAniStartTime(GetTickCount());
		break;
	}
	case SIMON_STATE_IDLE:
	{
		isStanding = true;
		vx = 0;
		break;
	}
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	}
}
void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	{
		right = x +  SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void Simon::Reset()
{
	SetState(SIMON_STATE_IDLE);
	SetSpeed(0, 0);
}
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	whip->Update(dt, coObjects);
	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x+= dx;
		y+= dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Torch*>(e->obj))
			{
				DebugOut(L"Collision Simon and torch %d %d\n", e->nx, e->ny);
				//Process normally
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny != 0)
				{
					vy = 0;
					isStanding = true;
				}
			}
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			
			 if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			 
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//check collsion when simon attack
	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_AND_ATTACK)
	{
		whip->SetOrientation(nx);
		whip->SetWhipPosition(D3DXVECTOR2(x, y), isStanding);
	}
	
}
