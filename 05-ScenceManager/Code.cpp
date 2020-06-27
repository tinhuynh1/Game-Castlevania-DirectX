#include "Code.h"
#include "Sprites.h"

void Code::DrawNumber(int number, D3DXVECTOR2 position, int length)
{
	string str = to_string(number);

	//Dùng khi tăng điểm thì sẽ trừ bớt số 0 thay thế là điểm ở hàm dưới(Nếu không có thì sẽ thêm điểm ở phía sau vầ giữ nguyên length
	for (int i = 0; i < length - str.size(); i++)
	{
		CSprites::GetInstance()->Get(77000)->Draw(position.x, position.y,-1);
		position.x += 7;
	}

	//Đếm có bao nhiêu chữ số và ghi điểm
	for (int i = 0; i < str.size(); i++)
	{
		CSprites::GetInstance()->Get(77000 + Convert(str[i]))->Draw(position.x, position.y,-1);
		position.x += 7;
	}
}
void Code::DrawSubWeapon(D3DXVECTOR2 position, int info)
{
	switch (info)
	{
	case ID_DAGGER:
		CSprites::GetInstance()->Get(ID_DAGGER)->Draw(position.x, position.y, 1);
		break;
	case ID_BOOMERANG:
		CSprites::GetInstance()->Get(ID_BOOMERANG)->Draw(position.x, position.y, 1);
		break;
	case ID_HOLYWATER:
		CSprites::GetInstance()->Get(ID_HOLYWATER)->Draw(position.x-2, position.y-2, 1);
		break;
	case ID_AXE:
		CSprites::GetInstance()->Get(ID_AXE)->Draw(position.x, position.y, 1);
		break;
	case ID_WATCH:
		CSprites::GetInstance()->Get(ID_WATCH)->Draw(position.x, position.y-3, 1);
		break;
	default:
		break;
	}
}
void Code::DrawHP(D3DXVECTOR2 position, int health, int kind)
{
	CSprites* sprites = CSprites::GetInstance();
	CSprite* spriteHealthMinus = sprites->Get(77102);
	CSprite* spriteHealth = NULL;
	int healthGeneral = NUM_HEALTH;

	if (kind == NUM_ID_SIMON)
		spriteHealth = sprites->Get(77101); //HEALTH SIMON
	else
		spriteHealth = sprites->Get(77103); //HEALTH BOSS

	for (int i = 0; i < health; i++)
	{
		spriteHealth->Draw(position.x, position.y, -1);
		position.x += 5;
	}


	for (int i = 0; i < healthGeneral - health; i++)
	{
		spriteHealthMinus->Draw(position.x, position.y, -1);
		position.x += 5;
	}

	sprites = NULL;
	spriteHealthMinus = NULL;
	spriteHealth = NULL;
}

int Code::Convert(char c)
{
	return c - '0';
}