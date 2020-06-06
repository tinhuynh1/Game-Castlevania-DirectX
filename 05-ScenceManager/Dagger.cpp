#include "Dagger.h"
Dagger::Dagger() : CGameObject()
{
	vx = DAGGER_SPEED;
}
void Dagger::Render()
{
	
	animation_set->at(0)->Render(x, y, nx);
}
void Dagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (nx < 0) vx = -DAGGER_SPEED;
	else vx = DAGGER_SPEED;
	vy = 0;
	CGameObject::Update(dt);
	x += dx; 
}

void Dagger::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + DAGGER_WEAPON_BBOX_WIDTH;
	bottom = y + DAGGER_WEAPON_BBOX_HEIGHT;
}
RECT Dagger::GetBound()
{
	return CGameObject::GetBound();
}