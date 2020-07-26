#include "Fleaman.h"
#include "Brick.h"
Fleaman::Fleaman(Simon* simon)
{
	mSimon = simon;
	this->SetState(FLEAMAN_STATE_READY);
	this->isJumping = false;
}
Fleaman::~Fleaman()
{
}
void Fleaman::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isOutOfCamera == true)
	{
		//SetVisible(false);
		return;
	}
	CGameObject::Update(dt, coObjects);
	if (this->GetState() != FLEAMAN_STATE_READY && IsOutofCamera()==true)
	{
		SetVisible(false);
	}
	if (this->GetState() == FLEAMAN_STATE_JUMP)
	{
		vy += FLEAMAN_GRAVITY * dt;
		vx = (nx > 0) ? FLEAMAN_JUMP_SPEED_X : -FLEAMAN_JUMP_SPEED_X;
	}
	if (timerToReady > 1000 && GetState()==FLEAMAN_STATE_READY)
	{
		SetState(FLEAMAN_STATE_JUMP);
		timerToReady = 0;
	}
	else
	{
		timerToReady += dt;
	}
	
	if (abs(mSimon->GetInstance()->GetPosition().x - this->x) > 64)
	{
		if (IsOnGround() == true && mSimon->untouchable==0)
		{
			if (mSimon->GetInstance()->GetPosition().x < this->x)
			{
				nx = -1;
			}
			else
			{
				nx = 1;
			}
		}
	}
	if (isStop)
	{
		vx = 0;
		vy = 0;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
		SetOnGround(false);
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;
		if (ny <= 0)
			y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (nx != 0)
				{
					x += dx;
					nx = -nx;
				}
				if (e->ny != 0)
				{
					isCanReturn = true;
					if (e->ny == -1)
					{
						SetOnGround(true);
						vy = 0;
					}
					else
					{
						SetOnGround(false);
						y += dy;
					}
				}
				else
				{
					isCanReturn = false;
				}
				if (isJumping)
				{	
					if (abs(mSimon->GetInstance()->GetPosition().x - this->x) < 48)
						vy = -0.2f;
					else
						vy = -0.05f;
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}
void Fleaman::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLEAMAN_STATE_READY:
	{
		vx = 0;
		vy = 0;
		break;
	}
	case FLEAMAN_STATE_JUMP:
	{
		isJumping = true;
		//vx = (nx > 0) ? FLEAMAN_JUMP_SPEED_X : -FLEAMAN_JUMP_SPEED_X;
		break;
	}
	}
	this->state = state;
}
void Fleaman::Render()
{
	if (!isStop)
	{
		if(this->GetState()==FLEAMAN_STATE_READY)
		animation_set->at(0)->Render(x, y, nx);
		else
		{
			//không đụng gạch thì frame duỗi chân
			if (IsOnGround() == true)
			{
				animation_set->at(0)->RenderByFrame(0, nx, x, y);
			}
			else
			{
				animation_set->at(0)->RenderByFrame(1, nx, x, y);
			}
		}
	}
	else
	{
		animation_set->at(0)->RenderByFrame(animation_set->at(0)->GetCurrentFrame(), nx, x, y);
	}
	//RenderBoundingBox();
}

RECT Fleaman::GetBound()
{
	return CGameObject::GetBound();
}

void Fleaman::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	{
		left = x;
		top = y;
		right = x + FLEAMAN_BBOX_WIDTH;
		bottom = y + FLEAMAN_BBOX_HEIGHT;
	}

}
