#include "Ghost.h"
Ghost::Ghost(Simon* simon)
{
	this->SetVisible(false);
	mSimon = simon;
	this->healthPoint = 3;
}
Ghost::~Ghost()
{
}
void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	//vy += 0.0018f * dt;
	vy = 0;
	vx = (nx > 0) ? 0.052f : -0.052f;
	if (mSimon->GetInstance()->GetPosition().x < 576 && this->healthPoint>0)
	{
		SetVisible(true);
	}
	if (abs(mSimon->GetInstance()->GetPosition().x - x) > 32)
	{
		if (mSimon->GetInstance()->GetPosition().x < x)
		{
			nx = -1;
		}
		else
		{
			nx = 1;
		}
	}
	if (isVisible() == false)
	{
		return;
	}
	else
	{
		x += dx;
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
}

void Ghost::Render()
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

RECT Ghost::GetBound()
{
	return CGameObject::GetBound();
}

void Ghost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GHOST_BBOX_WIDTH;
	bottom = y + GHOST_BBOX_HEIGHT;
}
