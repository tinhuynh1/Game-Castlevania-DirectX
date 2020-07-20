#include "Bat.h"
#include "Simon.h"
Bat::Bat(float x, float y) : CGameObject()
{
	start_x = x;
	start_y = y;
	vx = vy = 0;
	this->healthPoint = 1;
	SetState(BAT_STATE_IDLE);
}

void Bat::Render()
{
	int ani = -1;
	if (state == BAT_STATE_IDLE)
		ani = 0;
	else
		ani = 1;
	if (!isStop)
		animation_set->at(ani)->Render(x, y, nx);
	else
	{
		animation_set->at(ani)->RenderByFrame(animation_set->at(ani)->GetCurrentFrame(), nx, x, y);
	}
	//RenderBoundingBox();
}

void Bat::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BAT_STATE_IDLE:
	{
		vx = vy = 0;
		break;
	}
	case BAT_STATE_FLYING_DOWN:
	{
		vx = 0.08f;
		vy = 0.08f;
		break;
	}
	case BAT_STATE_FLYING_HORIZONTAL:
	{
		vx = 0.1f;
		vy = 0.0f;
		break;
	}
	}
}

void Bat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Activating Bat logic
	float xS, yS;
	Simon::GetInstance()->GetPosition(xS, yS);

	float xB, yB;
	this->GetPosition(xB, yB);

	if (xS - xB <= POINT_ACTIVE_BAT_X && yS - yB <= POINT_ACTIVE_BAT_Y) // Active Point 
		SetState(BAT_STATE_FLYING_DOWN);

	if (y - start_y >= BAT_FLYING_DOWN_DY)	// Redirecting point
		SetState(BAT_STATE_FLYING_HORIZONTAL);

	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (isStop)
	{
		vx = 0;
		vy = 0;
	}
	if (coEvents.size() == 0)
	{
		if (!isStop)
		{
			y += dy;
			x += dx;
		}
	}
	else
	{
		float min_tx, min_ty, nx, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx;
		y += min_ty * dy;

	}
	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void Bat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BAT_BBOX_WIDTH;
	bottom = y + BAT_BBOX_HEIGHT;
}
