#pragma once
#include <vector>
#include "GameObject.h"
#include <set>
#include "define.h"
#include "Game.h"
#include "Utils.h"

#define CELL_WIDTH 270
#define CELL_HEIGHT 270 
class Grid
{
public:
	vector<LPGAMEOBJECT> cells[2][5];
	set<CGameObject*> listTemp1;  //set: tồn tại 1 object duy nhất đó và ko trùng

	int rowIndex;
	int colIndex;
public:
	Grid()
	{
	}
	void GetListCollisionFromGrid(vector<CGameObject*>& listColObjects);
	void TakeObjectsFromCell(int rowIndex, int colIndex, vector<CGameObject*>& listColObjects);
	~Grid();
};

