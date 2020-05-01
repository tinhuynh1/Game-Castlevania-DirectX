#include "Whip.h"
Whip::Whip() :CGameObject()
{
}
void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (nx < 0)
	{
		left = x;
		top = y;
		right = x - WHIP_BBOX_WIDTH;
		bottom = y + WHIP_BBOX_HEIGHT;
	}
	else
	{
		left = x + 70;
		top = y;
		right = left + WHIP_BBOX_WIDTH;
		bottom = y + WHIP_BBOX_HEIGHT;
	}
}
void Whip::Render(int currentFrame)
{
	CAnimationSets::GetInstance()->Get(5)->at(WHIP_ANI)->RenderByFrame(currentFrame, nx, x, y);
	RenderBoundingBox();
}

void Whip::SetState(int state)
{

}
void Whip::SetWhipPosition(D3DXVECTOR2 simonPos, bool isStanding)
{
	if (isStanding)
	{
		if (nx > 0)
		{
			simonPos.x -= 49.0f;
			simonPos.y -= 3.0f;
		}
		else
		{
			simonPos.x -= 53.0f;
			simonPos.y -= 2.0f;
		}
	}


	SetPosition(simonPos.x, simonPos.y);
}