#include "Map.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
 
Map::Map(int _id,int _translate_y, int _translate_x)
{
	ID = _id;
	translate_y = _translate_y;
	translate_x = _translate_x;
}
void Map::LoadMap(const char* filename)
{
	ifstream inp(filename, ios::in);
	inp >> mapRows >> mapColumns >> tileWidth >> tileHeight >> tileColumns >> tileRows;
	for (int i = 0; i < mapRows; i++)
		for (int j = 0; j < mapColumns; j++)
			inp >> tileMap[i][j];
	inp.close();
}
void Map::DrawMap()
{
	float cam_x = CGame::GetInstance()->GetCamPos().x;
	float cam_y = CGame::GetInstance()->GetCamPos().y;

	float remain_x = fmod(cam_x, tileWidth);
	float remain_y = fmod(cam_y, tileHeight);

	// draw tiles within the viewport only
	int colCamLeft = cam_x / tileWidth;
	int rowCamTop = cam_y / tileHeight;
	int colCamRight = colCamLeft + SCREEN_WIDTH / tileWidth + SCREEN_WIDTH/2;
	
	int rowCamBottom = rowCamTop + SCREEN_HEIGHT / tileHeight;

	for (int j = colCamLeft; j <= colCamRight; j++)
	{
		for (int i = rowCamTop; i < rowCamBottom; i++)
		{
			float pos_x = (j - colCamLeft) * tileWidth - remain_x + translate_x-100;
			float pos_y = (i - rowCamTop) * tileHeight - remain_y + translate_y;

			RECT rectTile;
			int index = tileMap[i][j];
			rectTile.left = (index % tileColumns) * tileWidth;
			rectTile.top = (index / tileColumns) * tileHeight;
			rectTile.right = rectTile.left + tileWidth;
			rectTile.bottom = rectTile.top + tileHeight;

			CGame::GetInstance()->Draw(pos_x, pos_y,-1, CTextures::GetInstance()->Get(30), rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);
		}
	}
}
