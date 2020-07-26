#include "Raven.h"
Raven::Raven()
{
	this->SetVisible(false);
	this->healthPoint = 1;
	this->SetState(RAVEN_STATE_READY);
}
Raven::~Raven()
{
}
void Raven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isOutOfCamera == true)
	{
		//SetVisible(false);
		return;
	}
	CGameObject::Update(dt, coObjects);
	CGameObject::SetState(state);
	if (abs(Simon::GetInstance()->GetPosition().y - this->y) <= 4)
	{
		SetState(RAVEN_STATE_FLY_HORIZENTAL);
	}
	if (Simon::GetInstance()->GetPosition().x > 168 && healthPoint > 0)
	{
		this->SetVisible(true);
	}
	if (Simon::GetInstance()->GetPosition().x > 170 && state == 0)
	{
		SetState(RAVEN_STATE_FLY_CROSS);
	}
	if ((abs(Simon::GetInstance()->GetPosition().x - this->x) > 32) && this->GetState()== RAVEN_STATE_FLY_HORIZENTAL)
	{
		if (Simon::GetInstance()->GetPosition().x < this->x)
		{
			nx = -1;
		}
		else
		{
			nx = 1;
		}
	}
	//vy += 0.0018f * dt;
	//vy = 0;
	if (isVisible() == false)
	{
		return;
	}
	else
	{
		x += dx;
		y += dy;
	}
	if (start_untouchable != 0)
	{
		Untouchable();
	}
	if (isStop)
	{
		vx = 0;
		vy = 0;
	}
	switch (state)
	{
	case RAVEN_STATE_READY:
	{
		vx = 0;
		vy = 0;
		break;
	}
	case RAVEN_STATE_FLY_CROSS:
	{
		vx = (nx > 0) ? 0.052f : -0.052f;
		vy = (this->y < Simon::GetInstance()->GetPosition().y) ? 0.052f : -0.052f;
		if (timerToCros < 600)
		{
			timerToCros += dt;
		}
		else
		{
			timerToCros = 0;
			SetState(RAVEN_STATE_FLY_AIM);
		}
		break;
	}
	case RAVEN_STATE_FLY_HORIZENTAL:
	{
		vx = (nx > 0) ? 0.052f : -0.052f;
		vy = 0;
		break;
	}
	case RAVEN_STATE_FLY_AIM:
	{
		vx = 0;
		vy = 0;
		if (timerToAim < 400)
		{
			timerToAim += dt;
		}
		else
		{
			timerToAim = 0;
			SetState(RAVEN_STATE_FLY_CROSS);
		}
		break;
	}
	}
}

void Raven::Render()
{
	if (isVisible() == false)
	{
		return;
	}
	if (!isStop)
		animation_set->at(state)->Render(x, y, nx);
	else
	{
		animation_set->at(state)->RenderByFrame(animation_set->at(state)->GetCurrentFrame(), nx, x, y);
	}
	//RenderBoundingBox();
}

RECT Raven::GetBound()
{
	return CGameObject::GetBound();
}

void Raven::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;
}
