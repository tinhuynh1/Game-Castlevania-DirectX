#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Simon.h"
#include "Torch.h"
#include "HeartItem.h"
#include "ChainItem.h"
#include "DaggerItem.h"
#include "Dagger.h"
#include "Map.h"
#include "HeartItem.h"
#include "Items.h"
#include "BotStair.h"
#include "TopStair.h"
#include "Knight.h"
#include "MovingPlatform.h"
#include "BoomerangItem.h"
#include "Boomerang.h"
#include "BreakableBrick.h"
#include "CrownItem.h"
#include "Board.h"
class CPlayScene: public CScene
{
public: 

	bool isReturn = false;
	int map_width;
	int scene_id;
	Simon *player;					// A play scene has to have player, right? 
	Dagger* dagger;
	Boomerang* boomerang;
	Board* HUD;
	int tileMapLineY = 0;
	int tileColumns, tileRows;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> listStair;
	vector<LPGAMEOBJECT> listBrick;
	vector<LPGAMEOBJECT> listTorch;
	vector<LPGAMEOBJECT> listItem;
	vector<LPGAMEOBJECT> listPortal;
	vector<LPGAMEOBJECT> listEnemy;

	vector<LPMAP> tileMap;
	void _ParseSection_TILE_SHEET(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP_INFO(string line);
	void _ParseSection_MAP(string line);
public: 
	 
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void CheckCollision_ItemAndSimon();
	void CheckCollision_TorchAndSimon();
	void CheckCollision_PortalAndSimon();
	void CheckCollision_DaggerAndTorch();
	void CheckCollision_SimonAndBrick();
	void CheckCollision_SimonAndBoomerang();
	void CheckCollision_BoomerangAndEnemy();
	Simon * GetPlayer() { return player; } 
	Dagger* GetDagger() { return dagger; }
	Boomerang* GetBoomerang() { return boomerang; }
	vector<LPGAMEOBJECT> GetObjects() { return objects; };
	vector<LPGAMEOBJECT> GetListStair() { return listStair; };
	vector<LPGAMEOBJECT> GetListBrick() { return listBrick; };
	vector<LPGAMEOBJECT> GetListTorch() { return listTorch; };
	vector<LPGAMEOBJECT> GetListItem() { return listItem; };
	vector<LPGAMEOBJECT> GetListPortal() { return listPortal; };
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{

public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

