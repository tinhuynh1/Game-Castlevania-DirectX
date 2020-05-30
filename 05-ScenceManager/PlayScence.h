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
#include "Map.h"
#include "HeartItem.h"
#include "Items.h"
#include "BotStair.h"
#include "TopStair.h"
#include "Knight.h"
class CPlayScene: public CScene
{
public: 
	 
	Simon *player;					// A play scene has to have player, right? 
	int tileMapLineY = 0;
	int tileColumns, tileRows;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> listStair;
	vector<LPGAMEOBJECT> listBrick;
	vector<LPGAMEOBJECT> listTorch;
	vector<LPGAMEOBJECT> listItem;
	vector<LPGAMEOBJECT> listPortal;
	vector<LPMAP> tileMap;
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
	Simon * GetPlayer() { return player; } 
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

