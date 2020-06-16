#pragma once
#include<string>
#include <d3dx9.h>
#include <d3d9.h>
#include "define.h"
#define NUM_HEALTH 16
#define NUM_SCORE_HEART 5
#define NUM_ID_SIMON 0
#define NUM_ID_BOSS 1
using namespace std;

class Code
{
public:
	void DrawNumber(int number, D3DXVECTOR2 position, int length);
	void DrawSubWeapon(D3DXVECTOR2 position, int info);
	void DrawHP(D3DXVECTOR2 position, int health, int kind);
	int Convert(char c);
};
