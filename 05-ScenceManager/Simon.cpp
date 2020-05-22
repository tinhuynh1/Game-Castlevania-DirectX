#include "Simon.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"
#include "Portal.h"
#include "Torch.h"
#include "Brick.h"
#include "BotStair.h"


Simon* Simon::__instance = NULL;


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
	else if (state == SIMON_STATE_ONSTAIR)
	{
		if (isUpstair)
		{
			if (isStopOnStair)
			{
				ani = SIMON_ANI_UPSTAIR_STOP; // ani = 6;
			}
			else
			{
				ani = SIMON_ANI_UPSTAIR; //ani = 7
			}
		}
		else if (isUpstair == false)
		{
			if (isStopOnStair)
			{
				ani = SIMON_ANI_DOWNSTAIR_STOP; //ani = 8;
			}
			else
			{
				ani = SIMON_ANI_DOWNSTAIR; //ani = 9;
			}
		}
	}
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
	
	RenderBoundingBox();
	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_AND_ATTACK)
		whip->Render(animation_set->at(ani)->GetCurrentFrame());
	//RenderBoundingBox();
}
void Simon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SIMON_STATE_ONSTAIR:
		if (isUpstair)
		{
			nx = StairDirection;
			if (nx > 0)
			{
				//simon đi lên hướng phải
				vx = 0.008f;
				vy = -0.008f;
			}
			else
			{
				//simon đi lên hướng trái
				vx = -0.008f;
				vy = -0.004f;
			}
		}
		else if (isUpstair == false)
		{
			nx = -StairDirection;
			if (nx > 0)
			{
				//simon xuống hướng phải
				vx = 0.008f;
				vy = 0.008f;
			}
			else
			{
				//simon đi xuống hướng trái
				vx = -0.008f;
				vy = 0.008f;
			}
		}
		if (isStopOnStair)
		{
			//đang đứng ở cầu thang
			vx = 0;
			vy = 0;
			isSitAttack = false;
			isAttack = false;
		}
		//////////////////
		break;
	case SIMON_STATE_WALKING:
		if (isWalkingToStair)
		{
			if (nx > 0)
				vx = SIMON_WALKING_TO_STAIR_SPEED;
			else
				vx = -SIMON_WALKING_TO_STAIR_SPEED;
		}
		else
		{
			if (nx > 0)
			{
				vx = SIMON_WALKING_SPEED;
			}
			else
			{
				vx = -SIMON_WALKING_SPEED;
			}
		}
			break;
	case SIMON_STATE_SIT:
	{
		isStanding = false;
		vx = 0;
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
		isSitAttack = false;
		isAttack = false;
		vx = 0;
		vy = 0;
		break;
	}
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	}
}
void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == SIMON_STATE_SIT)
	{
		left = x;
		top = y + 6;
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT+2;
	}
	else
	left = x;
	top = y;
	right = x +  SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void Simon::Reset()
{
	SetState(SIMON_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}
RECT Simon::GetBound()
{
	RECT rect;
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	rect.left = l;
	rect.top = t;
	rect.right = r;
	rect.bottom = b;
	return rect;
}
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	whip->Update(dt, coObjects);
	// Simple fall down
	//vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (GetState() == SIMON_STATE_ATTACK)
	{
		
	}
	//simon không ở trên cầu thang
	if (!isOnStair)
	{
		vy += SIMON_GRAVITY * dt;
		//CheckCollisionWithGround(dt, coObjects);

		isWalkingToStair = false;  //chỉnh speed lại thành đi bth 
	}
	else
	{
		x += dx;
		y += dy;
	}
	if (isOnStair)
	{
		if (isHitTopStair == true && isUpstair == true)
		{
			CheckCollisionOnStair(dt, coObjects);
		}
		else if (isHitBottomStair == true && isUpstair == false)
		{
			CheckCollisionOnStair(dt, coObjects);
		}
	}

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
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;



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
			else if (dynamic_cast<BotStair*>(e->obj))
			{
				//DebugOut(L"Collision Simon and Botstair %d %d\n", e->nx, e->ny);
				//Process normally
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny != 0) vy = 0;

			}
			else if (dynamic_cast<HeartItem*>(e->obj))
			{
				DebugOut(L"[ITEMS] Heart Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
				}
			}
			else if (dynamic_cast<ChainItem*>(e->obj))
			{

				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
					this->whip->LevelUp();
					DebugOut(L"[INFO] WHIP UPGRADED \n");
				}
			}
			else if (dynamic_cast<DaggerItem*>(e->obj))
			{
				DebugOut(L"[ITEMS] Dagger Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
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
		int ani = -1;
		if (state == SIMON_STATE_ATTACK)
			ani = SIMON_ANI_ATTACK;
		else
		{
			ani = SIMON_ANI_SIT_AND_ATTACK;
		}
		whip->SetOrientation(nx);
		whip->SetWhipPosition(D3DXVECTOR2(x, y), isStanding);
		if (animation_set->at(ani)->GetCurrentFrame() == 2)
		 
		{
			int count = 0;
			for (UINT i = 0; i < coObjects->size(); i++) {
				LPGAMEOBJECT temp = coObjects->at(i);

				if (dynamic_cast<Torch*>(temp))
				{
					Torch* torch = dynamic_cast<Torch*>(temp);
					float left, top, right, bottom;
					temp->GetBoundingBox(left, top, right, bottom);
					
					if (whip->isColliding(temp->GetBound()) == true)
					{
						DebugOut(L"[INFO]Whip Collision with Torch \n");
						temp->SetState(TORCH_DESTROYED);
						temp->animation_set->at(1)->SetAniStartTime(GetTickCount());
					}
				}
			}
		}
	}

}
	
void Simon::CheckCollisionWithGround(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	CalcPotentialCollisions(colliable_objects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx; //dx=vx*dt
		y += dy;

	}
	else
	{
		isOnGround = true;
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		//block 	
		x += min_tx * dx + nx * 0.2f;
		y += min_ty * dy + ny * 0.2f;


		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		if (ny == -1)
		{
			if (this->GetPosition().y - firstY >= 50 /*&& isLand == false*/)
			{
				isLand = true;
			}
			firstY = this->GetPosition().y;
		}

		if (ny == -1 && isJumping == true)
		{
			if (isAttack == false /*&& isHitEnemy == false*/) //set đk này để khi chạm đất mà đánh chưa xong thì phải hoàn thành hd đánh r mới đứng yên
				SetState(SIMON_STATE_IDLE);

			isJumping = false;
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Simon::CheckCollisionOnStair(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	y -= 60.0f;  //kéo vy lên 1 đoạn
	vy = 100000.0f;  //kéo xuống lại ngay lập tức 
	//(diễn ra trong 1 frame)

	CGameObject::Update(dt);

	coEvents.clear();

	CalcPotentialCollisions(colliable_objects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		y += min_ty * dy + ny * 0.2f;

		if (ny != 0)
		{
			vy = 0;
			vx = 0;
			isOnStair = false;
			isHitTopStair = false;
			isHitBottomStair = false;
			isStopOnStair = false;
			isWalkingToStair = false;
			SetState(SIMON_STATE_IDLE);
		}
	}

	vy = SIMON_GRAVITY * dt;
	CGameObject::Update(dt);
}
