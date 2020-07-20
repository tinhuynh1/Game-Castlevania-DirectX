#include "HolyWater.h"
HolyWater::HolyWater()
{
	this->visible = false;
	this->damage = 1;
}

HolyWater::~HolyWater()
{
}
void HolyWater::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	/*if (this->isVisible()==false)
	{
		return;
	}*/
	CGameObject::Update(dt, colliable_objects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (isVisible())
	{
		CalcPotentialCollisions(colliable_objects, coEvents);
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		//block 	
		x += min_tx * dx;
		y += min_ty * dy + ny * 0.2f;

		if (ny == -1)
		{
			DebugOut(L"OKKKKKKKKKKKKKKK\n");
			isOnGround = true;
			vy = 0;
			vx = 0;
		}

	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (isOnGround == false)
	{
		
		if (abs(x - GetFirstPos()) <= 25) //bay lên  //Nếu muốn ném xa/gần trc khi rơi xuống thì chỉnh số này
		{
			if (nx == 1)
			{
				vx = 0.1f;

			}
			else
			{
				vx = -0.1f;
			}
			vy = -0.02f;  //bay cao thì tăng vy

		}
		else //rơi xuống
		{

			if (nx == 1)
			{
				vx = 0.08f;
			}
			else
			{
				vx = -0.08f;
			}
			vy = 0.14f;
		}
		//this->SetPosition(x, y);
	}
	if (isOnGround)
	{
		if (timer < 1000)
			timer += dt;
		else
		{
			SetVisible(false);
			timer = 0;
			isOnGround = false;	
			return;
		}
	}

}

void HolyWater::Render()
{
	
	if (isOnGround)
		animation_set->at(1)->Render(x, y, nx);
	else
		animation_set->at(0)->Render(x, y, nx);
	//RenderBoundingBox();
}

void HolyWater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isOnGround)
	{
		left = x;
		top = y;
		right = x + 14;
		bottom = y + 15;
	}
	else
	{
		left = x;
		top = y;
		right = x + 6;
		bottom = y + 15;
	}
}

RECT HolyWater::GetBound()
{
	return CGameObject::GetBound();
}