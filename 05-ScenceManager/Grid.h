#pragma once
#include <vector>
#include "GameObject.h"
#include <set>
#include "define.h"
#include "Game.h"
#include "Utils.h"

#define CELL_WIDTH 80  //320/4
#define CELL_HEIGHT 60  //240/4
class Grid
{
private:

	vector<LPGAMEOBJECT> cells[10][200];
	set<CGameObject*> listTemp1;  //set: tồn tại 1 object duy nhất đó và ko trùng

	int rowIndex;
	int colIndex;
	bool isRevive = false;
public:

	Grid()
	{
	}


	void InsertIntoGrid(CGameObject* object);
	void CheckSimonRevive(bool isRevive);

	void GetListCollisionFromGrid(vector<CGameObject*>& listColObjects);
	void TakeObjectsFromCell(int rowIndex, int colIndex, vector<CGameObject*>& listColObjects);

	~Grid();
};

