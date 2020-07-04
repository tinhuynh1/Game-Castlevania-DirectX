#include "Ghost.h"
Ghost::Ghost()
{
}
Ghost::~Ghost()
{
}
void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isOutOfCamera == true)
		return;
	CGameObject::Update(dt, coObjects);
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
	if (coEvents.size() == 0)
	{

		x += dx; //dx=vx*dt
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		y += min_ty * dy + ny * 0.2f;

		if (!isStop)
		{
			// block 
			if (nx != 1)
				x += min_tx * dx + nx * 0.2f;;

			if (nx == 1)  //đụng trái	
				x += dx;
		}

		if (ny != 0) vy = 0;
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];


	if (nx > 0)
		vx = 0.052f;
	else
		vx = -0.052f;

}

void Ghost::Render()
{
	if (!isStop)
		animation_set->at(state)->Render(x, y, nx);
	else
	{
		animation_set->at(state)->RenderByFrame(animation_set->at(state)->GetCurrentFrame(), nx, x, y);
	}
	//RenderBoundingBox();
}

RECT Ghost::GetBound()
{
	return CGameObject::GetBound();
}

void Ghost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (nx > 0)
	{
		left = x - 16;
		top = y;
		right = x;
		bottom = y + 32;
	}
	else
	{
		left = x + 1;
		top = y;
		right = x + 16;
		bottom = y + 32;
	}
}
