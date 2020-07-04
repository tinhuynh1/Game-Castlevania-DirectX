#include "Axe.h"
Axe::Axe()
{
	this->visible = false;
	this->damage = 2;
}
Axe::~Axe()
{

}
void Axe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 15;
}

RECT Axe::GetBound()
{
	return CGameObject::GetBound();
}
void Axe::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (isVisible()==false)
	{
		return;
	}

	CGameObject::Update(dt);

	vy += 0.0005f * dt; //gravity
	y += dy;
	x += dx;

	if (abs(x - this->firstx) <= 12) //bay lên  //Nếu muốn ném xa/gần trc khi rơi xuống thì chỉnh số này
	{
		if (nx>0)
		{
			vx = 0.0093f * dt;
		}
		else
		{
			vx = -0.0093f * dt;
		}
		//x += dx;
		vy = -0.15f;

	}
	else //rơi xuống
	{
		if (nx>0)
		{
			vx = 0.009f * dt;
		}
		else
		{
			vx = -0.009f * dt;
		}

		//x += dx;
		if (abs(x - this->firstx) >= 70)  //quãng đg búa đi trc khi hạ xuống (để tạo vòng cung)
			vy = 0.3f;

	}
	if (this->y  > 195)
	{
		visible = false;
	}


	this->SetPosition(x, y);
}
void Axe::Render()
{
	if (this->visible==false)
		return;
	animation_set->at(0)->Render(x, y, nx);

}
