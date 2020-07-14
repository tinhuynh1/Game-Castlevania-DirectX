#include "Grid.h"
void Grid::InsertIntoGrid(CGameObject* object)
{
	D3DXVECTOR2 objPosition = object->GetPosition();
	 RECT r = object->GetBound();

	int objWidth = object->GetWidth();
	int objHeight = object->GetHeight();

	//o bat dau neu' nhu vat dai va` nam` tren nhieu o
	int rowInMapBegin = objPosition.y / (CELL_HEIGHT);
	int colInMapBegin = objPosition.x / (CELL_WIDTH);  

	//o ket thuc neu' nhu vat dai va` nam` tren nhieu o
	int rowInMapEnd = r.bottom / CELL_HEIGHT;
	int colInMapEnd = r.right / CELL_WIDTH; //


	//insert vào lưới

	for (int i = rowInMapBegin; i <= rowInMapEnd; i++)
	{
		for (int j = colInMapBegin; j <= colInMapEnd; j++)
		{
			cells[i][j].push_back(object);
		}
	}

}
void Grid::GetListCollisionFromGrid(vector<CGameObject*>& listColObjects)
{
	
	int rowCam = CGame::GetInstance()->GetCamPosition().y / (CELL_HEIGHT);
	int colCam = CGame::GetInstance()->GetCamPosition().x / (CELL_WIDTH);
	listColObjects.clear();
	listTemp1.clear();



	for (int j = 0; j <= SCREEN_HEIGHT / CELL_HEIGHT; j++)
	{
		for (int i = 0; i <= SCREEN_WIDTH / CELL_WIDTH; i++)
		{
			TakeObjectsFromCell(j + rowCam, i + colCam, listColObjects);

		}
	}

}

void Grid::TakeObjectsFromCell(int rowIndex, int colIndex, vector<CGameObject*>& listColObjects)
{
	for (int i = 0; i < cells[rowIndex][colIndex].size(); i++)
	{
		if (cells[rowIndex][colIndex].at(i)->isVisible()==false)
		{
			if (this->isRevive) //reload lại candle/torch khi Simon revive
			{
				cells[rowIndex][colIndex].at(i)->SetVisible(true);
			}
		}

		if (cells[rowIndex][colIndex].at(i)->isVisible()!=false)
		{
			if (listTemp1.find(cells[rowIndex][colIndex].at(i)) == listTemp1.end())
			{
				listTemp1.insert(cells[rowIndex][colIndex].at(i));
				listColObjects.push_back(cells[rowIndex][colIndex].at(i));
			}
		}

	}
}


void Grid::CheckSimonRevive(bool isRevive)
{
	this->isRevive = isRevive;
}


Grid::~Grid()
{
}