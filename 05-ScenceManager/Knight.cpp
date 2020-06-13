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
	//vy += 0.0018f * dt;
	x += dx;
	y += dy;
	vx = (nx > 0) ? BLACK_KNIGHT_WALKING_SPEED : -BLACK_KNIGHT_WALKING_SPEED;
	if (start_untouchable != 0)
	{
		Untouchable();
	}
	if (vx < 0 && x < 36) {
		nx = -nx;
		x = 36; vx = -vx;

	}

	if (vx > 0 && x > 122) {
		nx = -nx;
		x = 122; vx = -vx;
	}
	if (Simon::GetInstance()->isJumping == true && Simon::GetInstance()->GetPosition().y < 80)
	{
		nx = 1;
		vx = BLACK_KNIGHT_WALKING_SPEED;
	}

}
void Knight::Render()
{
	animation_set->at(state)->Render(x, y, nx);
	RenderBoundingBox();
}

Knight::Knight()
{
	this->healthPoint = 3;
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
RECT Knight::GetBound()
{
	return CGameObject::GetBound();
}