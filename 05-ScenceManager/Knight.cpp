#include "Knight.h"
void  Knight::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left +  KNIGHT_BBOX_WIDTH;
	bottom = top +  KNIGHT_BBOX_HEIGHT;
}
void Knight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;

	if (vx < 0 && x < 36) {
		nx = -nx;
		x = 36; vx = -vx;
	}

	if (vx > 0 && x > 122) {
		nx = -nx;
		x = 122; vx = -vx;
	}
}
void Knight::Render()
{
	animation_set->at(state)->Render(x, y, nx);
	RenderBoundingBox();
}

Knight::Knight()
{
	SetState(KNIGHT_STATE_WALKING);
}

void Knight::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KNIGHT_STATE_WALKING:
		vx = BLACK_KNIGHT_WALKING_SPEED;
		break;
	}

}