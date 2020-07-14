#include "BreakableBrick.h"
BreakableBrick::BreakableBrick()
{
	state = BREAKABLE_BRICK_STATE_ORIGIN;
}
void BreakableBrick::Render()
{

	animation_set->at(state)->Render(x, y, -1);
	//RenderBoundingBox();
}
void BreakableBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BREAKABLE_BRICK_BBOX_WIDTH;
	bottom = y + BREAKABLE_BRICK_BBOX_HEIGHT;
}
void BreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (isVisible() == false)
	{
		return;
	}
	CGameObject::Update(dt, coObject);
}
RECT BreakableBrick::GetBound()
{
	return CGameObject::GetBound();
}