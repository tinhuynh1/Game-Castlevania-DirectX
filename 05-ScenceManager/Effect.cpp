#include "Effect.h"

Effect::Effect()
{
}

Effect::~Effect()
{
}
void Effect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->visible==false)
	{
		return;
	}
	if (timer < 250)
		timer += dt;
	else
	{
		timer = 0;
		visible = false;
	}
}

void Effect::Render()
{
	if (this->visible==false)
	{
		return;
	}

	animation_set->at(0)->Render(x, y, nx);
}

