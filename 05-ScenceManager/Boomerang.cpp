#include "Boomerang.h"
#include "Simon.h"
#include "Brick.h"
Boomerang::Boomerang()
{
	SetDame(2);
}
void Boomerang::Render()
{
	animation_set->at(1)->Render(x, y, nx);
	//RenderBoundingBox();
}
void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx = 0.12f * nx;
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx;
		y += min_ty * dy;
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	float left, top, right, bottom;
	CGame::GetInstance()->CamereBoundingBox(left, top, right, bottom);
	if (!isReturn)
	{
		//boomerang đụng viền màn hình thì quay lại
		if (x > (right - 16) || x <= left)
		{
		//DebugOut(L"x boom is: %f , cx is: %f\n", boomerang->x, (cx + SCREEN_WIDTH-16));
			nx = -nx;
			isReturn = true;
		}
		//boomerang bay 1 khoảng 150 ki lô mét thì quay lại
		else if (abs(x - Simon::GetInstance()->GetPosition().x) > 150)
		{
		//DebugOut(L"x boom is: %f, x player is: %f \n", boomerang->x, player->x);
			nx = -nx;
			isReturn = true;
		}
	}
	else
	{
		if ((x > right) || (x <= left))
		{
			SetVisible(false);
			isReturn = false;
		}
	}
	if (isReturn)
	{
		if (Simon::GetInstance()->CheckCollision(this))
		{
			SetVisible(false);
			isReturn = false;
		}
	}
}
void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_WEAPON_BBOX_WIDTH;
	bottom = y + BOOMERANG_WEAPON_BBOX_HEIGHT;
}
RECT Boomerang::GetBound()
{
	return CGameObject::GetBound();
}