#include "CrownItem.h"


CrownItem::CrownItem()
{
	SetVisible(true);
}
void CrownItem::Render()
{
	if(y<176)
	animation_set->at(0)->Render(x, y, nx);	
	//RenderBoundingBox();
}
void CrownItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	// grow up
	float xS, yS;
	Simon::GetInstance()->GetPosition(xS, yS);


	if (y <= MAX_Y)
	{
		y = MAX_Y;
	}
	else
	{
	if (xS >= DISTANCE_SIMON_GROW_UP_X)
		{
			vy = -CROWN_GROWING_UP_SPEED;
		}

	}

	//// Update position
	x += dx;
	y += dy;
}
void CrownItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CROWN_BBOX_WIDTH;
	bottom = y + CROWN_BBOX_HEIGHT;
}