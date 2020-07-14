#include "Fleaman.h"
#include "Brick.h"
Fleaman::Fleaman(Simon* simon)
{
	mSimon = simon;
	this->SetState(FLEAMAN_STATE_READY);
	this->hopping = false;
}
Fleaman::~Fleaman()
{
}
void Fleaman::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isOutOfCamera == true)
		return;
	CGameObject::Update(dt, coObjects);
	vy += FLEAMAN_GRAVITY * dt;
	vx = (nx > 0) ? FLEAMAN_JUMP_SPEED_X : -FLEAMAN_JUMP_SPEED_X;
	if (mSimon->GetInstance()->GetPosition().x - this->x < 72 && GetState()==FLEAMAN_STATE_READY)
	{
		SetState(FLEAMAN_STATE_JUMP);
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
					if (e->ny == -1) vy = 0; // hunch back standing on brick
					else 	y += dy; //hunch back can jump through brick
				}

				if (hopping)
				{					
					vy = -0.15f;
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
		hopping = true;
		//vx = (nx > 0) ? FLEAMAN_JUMP_SPEED_X : -FLEAMAN_JUMP_SPEED_X;
		break;
	}
	}
	this->state = state;
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
		right = x + FLEAMAN_BBOX_WIDTH;
		bottom = y + FLEAMAN_BBOX_HEIGHT;
	}

}
