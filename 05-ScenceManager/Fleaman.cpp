#include "Fleaman.h"
#include "Brick.h"
Fleaman::Fleaman()
{

}
Fleaman::Fleaman(Simon* simon)
{
	mSimon = simon;
}
Fleaman::~Fleaman()
{
}
void Fleaman::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isOutOfCamera == true)
		return;
	CGameObject::Update(dt, coObjects);
	CGameObject::SetState(state);
	vy += 0.015f * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	// No collision occured, proceed normally
	if (isStop)
	{
		vx = 0;
		vy = 0;
	}
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx;
		y += min_ty * dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				// The limmied of the knight is the width of the bricks under its feet
				CBrick* b = dynamic_cast<CBrick*>(e->obj);
				if (e->ny != 0)
				{
					vy = 0;
					y += ny * 0.4f;
				}
				if (e->nx != 0)
				{
					ReDirection();
					y += ny * 0.4f;
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (mSimon->GetInstance()->GetPosition().x - x < 30 && state == 0) 
	{
		state = STATE_JUMP;
	}
	switch (state)
	{
		case STATE_IDLE:
		{
			vx = 0;
			vy = 0;
		}
		case STATE_JUMP:
		{
			if (nx > 0)
			{
				//vx = 0.052f;
				vy = 0.052f;
			}
		}
	}
}

void Fleaman::Render()
{
	if (!isStop)
		animation_set->at(0)->Render(x, y, nx);
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
		right = x + 16;
		bottom = y + 16;
	}

}
