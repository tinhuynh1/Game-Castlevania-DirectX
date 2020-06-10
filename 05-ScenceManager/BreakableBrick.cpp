#include "BreakableBrick.h"
BreakableBrick::BreakableBrick()
{
	state = BREAKABLE_BRICK_STATE_ORIGIN;
}
void BreakableBrick::Render()
{

	animation_set->at(state)->Render(x, y, -1);
	//RenderBoundingBox();
}
void BreakableBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BREAKABLE_BRICK_BBOX_WIDTH;
	bottom = y + BREAKABLE_BRICK_BBOX_HEIGHT;
}
void BreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	//CGameObject::Update(dt, coObject);
	//NOTE: simon đánh ra ngoài 1 lần để trả collision = false; 
	simon = Simon::GetInstance();
	if (state == BREAKABLE_BRICK_STATE_ORIGIN)
	{
		if (simon->animation_set->at(SIMON_ANI_ATTACK)->GetCurrentFrame() == 2 || (simon->animation_set->at(SIMON_ANI_SIT_AND_ATTACK)->GetCurrentFrame() == 2))
		{
			if (CheckCollision(simon->whip))
			{
				Rocks::GetInstance()->DropRock(this->x, this->y);
				state = BREAKABLE_BRICK_STATE_HALF_PART;
			}

		}
	}
	else if((simon->animation_set->at(SIMON_ANI_ATTACK)->GetCurrentFrame() == 2) || (simon->animation_set->at(SIMON_ANI_SIT_AND_ATTACK)->GetCurrentFrame() == 2))
	{
		if (!CheckCollision(simon->whip))
		{
			count++;
		}
		if(CheckCollision(simon->whip) && count != 0)
		{
			Rocks::GetInstance()->DropRock(this->x, this->y+16);
			visible = false;
		}
	}
}
RECT BreakableBrick::GetBound()
{
	return CGameObject::GetBound();
}