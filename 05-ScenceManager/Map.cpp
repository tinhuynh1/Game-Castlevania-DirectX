#include "Map.h"
void Map::Render()
{
	int currentMapId = CGame::GetInstance()->GetSceneId();
	LPDIRECT3DTEXTURE9 texture = CTextures::GetInstance()->Get(currentMapId);
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, -1,  texture, left, top, right, bottom, 255);
}