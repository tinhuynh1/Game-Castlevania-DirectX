#include "Boss.h"
Boss::Boss()
{
}
Boss::Boss(Simon* simon)
{
	mSimon = simon;
}
Boss::~Boss()
{
}
void Boss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	x += dx;
	y += dy;
	CGameObject::SetState(state);
	if (mSimon->GetInstance()->GetPosition().x > x && state == 0) //khi Simon đi gần dơi đang ngủ, nó thức dậy và tấn công
	{
		state = FLY_AFTER_SLEEP;
	}
	//Nếu boss đụng biên thì bay ngược lại
	if (state != 0)
	{
		if (x <= 512)
			vx = -vx;
		if (x >= 721)
			vx = -vx;
		if (y <= 30)
			vy = 0.03f;
	}
	switch (state)
	{
	case FLY_AFTER_SLEEP:
		if (y <= 90) //bay xuống 1 khoảng sau khi ngủ, trc khi attack
		{
			vx = 0.02f;
			vy = 0.03f;
		}
		else
		{
			vx = 0;
			vy = 0;
			state = 2;
		}
		break;
	case ATTACK:  //attack
		if (timerAttack < 3000)
		{
			timerAttack += dt;
		}
		else
		{
			timerAttack = 0;
			isAttack = true;
		}
		if ((abs(x - mSimon->GetInstance()->GetPosition().x) >= 70 && isHitSimon == false) || isAttack == true) //khi Simon cách boss 1 khoảng nào đó thì boss mới attack
		{
			if (mSimon->GetInstance()->GetPosition().x < x)
			{
				vx = -0.08f;
				vy = 0.078f;
				isHitSimon = true;
			}
			else
			{
				vx = 0.08f;
				vy = 0.078f;
				isHitSimon = true;
			}
			isAttack = false;
		}
		if (abs(y - mSimon->GetInstance()->GetPosition().y) <= 5) //khi boss xuống gần Simon
		{
			vy = 0; //ko cho boss xuống nữa
			if (abs(mSimon->GetInstance()->GetPosition().x - x) >= 5)
			{
				vy = -0.038f;
				state = FLY_AFTER_ATTACK;
			}
		}
		if (y - mSimon->GetInstance()->GetPosition().y >= 60) //dành cho TH Simon nhảy qua đầu dơi thì dơi ko bay xuống hoài
		{
			vy = -0.038f;
			state = FLY_AFTER_ATTACK;
		}
		break;
	case FLY_AFTER_ATTACK: //bay lên tự do
		if (timerFlyUp < 1200)
			timerFlyUp += dt;
		else
		{
			timerFlyUp = 0;
			state = FLY_TO_THE_MIDDLE;
		}
		break;

	case FLY_TO_THE_MIDDLE:
		if (x != 640)  //497 cạnh trái màn hình
		{
			if (x < 635)
			{
				vx = 0.07f;
				vy = -0.01f;
			}
			else
			{
				vx = -0.07f;
				vy = -0.01f;
			}
		}
		if (x >= 640 - 20 && x < 640 + 10) //khi vào đúng vị trí trong khoảng lý tưởng thì set qua state aim liền 
		{
			vx = 0;
			vy = 0;
			state = 5;
		}
		break;

	case AIM:
		if (this->y <= 100)  //cho boss bay xuống khi đang aim
			vy = 0.02f;
		else
			vy = 0;

		if (abs(x - mSimon->GetInstance()->GetPosition().x) > 60)
		{
			if (mSimon->GetInstance()->GetPosition().x < x)
				vx = -0.055f;
		}
		else
		{
			vx = 0;
		}

		isHitSimon = false;
		if (timerAim < 2000)
		{
			timerAim += dt;
		}
		else
		{
			state = ATTACK;
			timerAim = 0;
		}
		break;
	}
}
void Boss::Render()
{
	if (state == FLY_AFTER_SLEEP)
	{
		ani = 1;
	}
	animation_set->at(ani)->Render(x, y, nx);
	RenderBoundingBox();
}

RECT Boss::GetBound()
{
	return CGameObject::GetBound();
}
void Boss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 48;
	bottom = y + 23;
}