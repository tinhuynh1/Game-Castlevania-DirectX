#include "Grid.h"
void Grid::GetListCollisionFromGrid(vector<CGameObject*>& listColObjects)
{
	
	int rowCam = CGame::GetInstance()->GetCamPosition().y / (CELL_HEIGHT);
	int colCam = CGame::GetInstance()->GetCamPosition().x / (CELL_WIDTH);
	listColObjects.clear();
	listTemp1.clear();



	for (int j = 0; j <=SCREEN_HEIGHT / CELL_HEIGHT; j++) //1
	{
		for (int i = 0; i <= SCREEN_WIDTH / CELL_WIDTH; i++) //1
		{
			TakeObjectsFromCell(j + rowCam, i + colCam, listColObjects);
		}
	}

}

void Grid::TakeObjectsFromCell(int rowIndex, int colIndex, vector<CGameObject*>& listColObjects)
{
	for (int i = 0; i < cells[rowIndex][colIndex].size(); i++)
	{
		//DebugOut(L"Cells[0][0] size is: %d \n", cells[rowIndex][colIndex].size());
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
Grid::~Grid()
{
}