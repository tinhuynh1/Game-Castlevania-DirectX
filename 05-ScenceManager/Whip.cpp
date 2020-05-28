#include "Whip.h"
#define WHIP_ANI_SET 3

Whip* Whip::__instance = NULL;
Whip* Whip::GetInstance()
{
	if (__instance == NULL) __instance = new Whip();
	return __instance;
}
Whip::Whip() :CGameObject()
{
	SetState(0);
}
void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + 10;
	bottom = top + 6;
	switch (state)
	{
	case NORMAL_WHIP:
	{
		if (nx > 0) left = x + 70;
		else
		{
			left = x + 25;
		}
		right = left + 24;
		break;
	}
	case SHORT_CHAIN:
	{
		if (nx > 0) left = x + 70;
		else
		{
			left = x + 25;
		}
		right = left + 24;
		break;
	}
	case LONG_CHAIN:
	{
		if (nx > 0) left = x + 70;
		else
		{
			left = x + 15;
		}
		right = left + 38;
		break;
	}
	default:
		break;
	}
 
}
void Whip::Render(int currentFrame)
{
	CAnimationSets::GetInstance()->Get(WHIP_ANI_SET)->at(state)->RenderByFrame(currentFrame, nx, x, y);
	RenderBoundingBox();
}
bool Whip::isColliding(RECT rect_object)
{
	float whip_left, whip_top, whip_right, whip_bottom;
	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);
	return CGameObject::AABB(GetBound(), rect_object);
}
void Whip::SetState(int state)
{
	this->state = state;
}
void Whip::SetWhipPosition(D3DXVECTOR2 simonPos, bool isStanding)
{

		if (nx > 0)
		{
			simonPos.x -= 49.0f;
			if (isStanding) simonPos.y -= 3.0f;
			else
			simonPos.y += 3.0f;
		}
		else
		{
			simonPos.x -= 53.0f;
			if (isStanding) simonPos.y -= 3.0f;
			else simonPos.y += 3.0f;
		}

	SetPosition(simonPos.x, simonPos.y);
}
void Whip::LevelUp()
{
	if (state == NORMAL_WHIP) SetState(SHORT_CHAIN);
	else if (state == SHORT_CHAIN) SetState(LONG_CHAIN);

}
RECT Whip::GetBound()
{
	return CGameObject::GetBound();
}