#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Simon.h"
#include "Torch.h"
#include "Map.h"
#include "HeartItem.h"
#include "Items.h"
#include "BotStair.h"
#include "TopStair.h"
class CPlayScene: public CScene
{
public: 
	 
	Simon *player;					// A play scene has to have player, right? 
	int tileMapLineY = 0;
	int tileColumns, tileRows;
	vector<LPGAMEOBJECT> objects;
	vector<LPMAP> tileMap;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
public: 
	 
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	Simon * GetPlayer() { return player; } 
	vector<LPGAMEOBJECT> GetObjects() { return objects; };
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{

public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

