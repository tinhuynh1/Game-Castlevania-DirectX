#include "Ghoul.h"
Ghoul::Ghoul()
{
	healthPoint = 1;
}
Ghoul::~Ghoul()
{
}
void Ghoul::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isOutOfCamera == true)
		SetVisible(false);
	CGameObject::Update(dt, coObjects);
	vy += 0.015f * dt;
	vx = (nx > 0) ? GHOUL_WALKING_SPEED : -GHOUL_WALKING_SPEED;
	if (start_untouchable != 0)
	{
		Untouchable();
	}
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
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					ReDirection();
				}
			}
		}
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
}

void Ghoul::Render()
{
	if (!isStop)
		animation_set->at(state)->Render(x, y, nx);
	else
	{
		animation_set->at(state)->RenderByFrame(animation_set->at(state)->GetCurrentFrame(), nx, x, y);
	}
	RenderBoundingBox();
}

RECT Ghoul::GetBound()
{
	return CGameObject::GetBound();
}

void Ghoul::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x;
	top = y;
	right = x+GHOUL_BBOX_WIDTH;
	bottom = y + GHOUL_BBOX_HEIGHT;
}
