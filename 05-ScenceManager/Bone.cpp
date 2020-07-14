#include "Bone.h"
Bone::Bone()
{
	this->SetVisible(true);
}
Bone::~Bone()
{
}
void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += 0.0005f * dt; //gravity

	y += dy;
	x += dx;
	if (abs(x - this->firstx) <= 6) //bay lên  //Nếu muốn ném xa/gần trc khi rơi xuống thì chỉnh số này
	{
		if (nx > 0)
		{
			vx = 0.0035f * dt;
		}
		else
		{
			vx = -0.0035f * dt;
		}
		int r = rand() % 2;
		if (r == 0)
		{
			vy = -0.13f;
		}
		else
		{
			vy = -0.22f;
		}
		

	}
	else //rơi xuống
	{
		if (abs(x - this->firstx) >= 32)  //quãng đg búa đi trc khi hạ xuống (để tạo vòng cung)
			vy = 0.3f;
	}
}

void Bone::Render()
{
	if (isVisible() == false)
	{
		return;
	}
	if (!isStop)
		animation_set->at(0)->Render(x, y, nx);
	else
	{
		animation_set->at(0)->RenderByFrame(animation_set->at(0)->GetCurrentFrame(), nx, x, y);
	}
	//RenderBoundingBox();
}

void Bone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 15;
	bottom = y + 16;
}