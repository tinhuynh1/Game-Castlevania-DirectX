#include "Whip.h"
Whip::Whip() :CGameObject()
{
	SetState(0);
}
void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + 10;
	bottom = top + 6;
	if (nx > 0) left = x + 70;
	else left = x + 25;
	right = left + 24;
}
void Whip::Render(int currentFrame)
{
	CAnimationSets::GetInstance()->Get(5)->at(WHIP_ANI)->RenderByFrame(currentFrame, nx, x, y);
	RenderBoundingBox();
}
bool Whip::isColliding(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left, whip_top, whip_right, whip_bottom;
	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
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