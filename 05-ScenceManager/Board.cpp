#include "Board.h"

Board* Board::__instance = NULL;
void Board::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	Simon* simon = Simon::GetInstance();
	CGame* game = CGame::GetInstance();
	float cx, cy;
	game->GetCamPos(cx, cy);

	sprites->Get(70000)->Draw(cx, cy + 2, -1);
	//STATE
	code->DrawNumber(game->GetSceneId(), { cx + 234, cy + 8 }, 2);
	//HEALTH SIMON
	code->DrawHP({ cx + 60, cy + 18 }, simon->GetHealth(), NUM_ID_SIMON);
	code->DrawHP({ cx + 60, cy + 28 }, 16, NUM_ID_BOSS);
	//NUM LIFE
	code->DrawNumber(simon->GetNumLife(), { cx + 200, cy + 28 }, 2);
	//TIME
	code->DrawNumber(GetTimeBoard(), { cx + 150, cy + 8 }, 4);
	//SCORE HEART
	code->DrawNumber(simon->GetNumHeart(), { cx + 200, cy + 18 }, 2);
	//SUB WEAPON
	code->DrawSubWeapon({ cx + 157, cy + 19 }, simon->GetSubWeapon());
	//SCORE
	code->DrawNumber(simon->GetScore(), { cx + 55, cy + 8 }, 6 );
}
Board::Board()
{
	timeBoard = NUM_TIME;
	timecount = GetTickCount();
}

Board* Board::GetInstance()
{
	if (__instance == NULL)
		__instance = new Board();

	return __instance;
}

void Board::Update(DWORD dt)
{
	//SET TIME
	if (GetTickCount() - timecount > NUM_COUNT_TIME&& timecount > 0)
	{
		timeBoard--;
		timecount = GetTickCount();
	}
}
