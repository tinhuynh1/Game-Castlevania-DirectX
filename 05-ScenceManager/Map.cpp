#include "Map.h"
void Map::Render()
{
	LPDIRECT3DTEXTURE9 texture = CTextures::GetInstance()->Get(ID_TILE_MAP);
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, -1,  texture, left, top, right, bottom, 255);
}