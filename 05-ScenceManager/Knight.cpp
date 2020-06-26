#include "Knight.h"
void  Knight::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left +  KNIGHT_BBOX_WIDTH;
	bottom = top +  KNIGHT_BBOX_HEIGHT;
}
void Knight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += 0.0018f * dt;
	vx = (nx > 0) ? BLACK_KNIGHT_WALKING_SPEED : -BLACK_KNIGHT_WALKING_SPEED;
	if (start_untouchable != 0)
	{
		Untouchable();
	}
	
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

				this->start_x = b->x + 1;
				if (e->ny != 0)
				{
					vy = 0;
					y += ny * 0.4f;
				}
				
			}

		}
	}
	//Update knight khi đang ở scene 2
	if (CGame::GetInstance()->GetSceneId() == 2)
	{
		if ((count / 2) % 2 == 0 && isNormal == true)
		{
			if (x >= MAX_X1 || x <= start_x)
			{
				ReDirection();
				count++;

			}
		}
		else
		{
			if (x >= MAX_X2 || x <= start_x)
			{
				ReDirection();
				count++;

			}
		}
	}
	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
void Knight::Render()
{
	animation_set->at(state)->Render(x, y, nx);
	//RenderBoundingBox();
}

Knight::Knight(float x, float y)
{
	this->start_x = x;
	this->start_y = y;
	this->healthPoint = 3;
	SetState(KNIGHT_STATE_WALKING);
}

void Knight::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KNIGHT_STATE_WALKING:
		vx = BLACK_KNIGHT_WALKING_SPEED;
		break;
	}

}
RECT Knight::GetBound()
{
	return CGameObject::GetBound();
}