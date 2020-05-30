#include "ChainItem.h"
#include "Brick.h"

ChainItem::ChainItem()
{
	this->visible = false;
}
void ChainItem::Render()
{
	animation_set->at(0)->Render(x, y, nx);
	//RenderBoundingBox();
}
void ChainItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += ITEM_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx;
		y += min_ty * dy;

		for (UINT i = 0; i < coEventsResult.size(); ++i)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				// Block brick
				if (e->ny != 0)
				{
					y += 0.4f * e->ny;
					vy = 0;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void ChainItem::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + CHAIN_BBOX_WIDTH;
	bottom = y + CHAIN_BBOX_HEIGHT;
}