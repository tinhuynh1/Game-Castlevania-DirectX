#include "Skeleton.h"
#include "Brick.h"
Skeleton::Skeleton()
{
}
Skeleton::~Skeleton()
{
}
void  Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 16;
	bottom = top + 32;
}
void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isOutOfCamera)
	{
		return;
	}
	CGameObject::Update(dt, coObjects);
	vy += 0.0018f * dt;
	vx = 0;
	if (start_untouchable != 0)
	{
		Untouchable();
	}

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
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

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
					y += ny * 0.4f;
				}
			}
		}
	}
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
void Skeleton::Render()
{
	if (!isStop)
		animation_set->at(0)->Render(x, y, nx);
	else
	{
		animation_set->at(0)->RenderByFrame(animation_set->at(0)->GetCurrentFrame(), nx, x, y);
	}
	//RenderBoundingBox();
}
RECT Skeleton::GetBound()
{
	return CGameObject::GetBound();
}