#include "Effect.h"
Effect::Effect()
{

	this->visible = false;
}

Effect::~Effect()
{
}
void Effect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::SetState(state);
	if (this->visible == false)
	{
		return;
	}
	if (timer < 300)
		timer += dt;
	else
	{
		timer = 0;
		visible = false;
	}
	switch (state)
	{
	case EFFECT_STATE_FLAME:
	{
		this->state = 0;
		break;
	}
	case EFFECT_STATE_100:
	{
		this->state = 1;
		break;
	}
	case EFFECT_STATE_400:
	{
		this->state = 2;
		break;
	}
	case EFFECT_STATE_700:
	{
		this->state = 3;
		break;
	}
	case EFFECT_STATE_2000:
	{
		this->state = 4;
		break;
	}
	case EFFECT_STATE_DESTROY:
	{
		this->state = 5;
		break;
	}
	}
}

void Effect::Render()
{
	if (this->visible == false)
	{
		return;
	}
	animation_set->at(state)->Render(x, y, -1);
}
void Effect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}
