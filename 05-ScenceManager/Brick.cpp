#include "Brick.h"

void CBrick::Render()
{
	//animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}
RECT CBrick::GetBound()
{
	return CGameObject::GetBound();
}