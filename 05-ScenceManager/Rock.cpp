#include "Rock.h"
#define ROCK_GRAVITY	0.0005f
Rock::Rock()
{
	SetVisible(false);
}
void Rock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt,coObject);
	vy += ROCK_GRAVITY * dt;
	x += dx;
	y += dy;
	if (y >= 200) SetVisible(false);
}
void Rock::Render()
{
	animation_set->at(state)->Render(x, y, -1);
}
void Rocks::AddRock(Rock* rock)
{
	rock->SetVisible(false);
	rocks.push_back(rock);
}
void Rocks::DropRock(float x, float y)
{
	Rock* piece = NULL;
	UINT i = 0;

	for (int j = 0; j < 4; j++) 
	{
		for (; i < rocks.size(); i++)
		{
			if (rocks[i]->isVisible() == false)
			{
				piece = rocks[i];
				i++;
				break;
			}
		}
		if (piece != NULL)
		{
			piece->SetPosition(x, y);
			piece->SetVisible(true);
			float vx = (float)(-100 + rand() % 200) / 1000;
			float vy = (float)(-100 + rand() % 200) / 1000;
			piece->SetSpeed(vx, -vy);
		}
	}

}

Rocks* Rocks::__instance = NULL;
Rocks* Rocks::GetInstance()
{
	if (__instance == NULL)
		__instance = new Rocks();
	return __instance;
}
