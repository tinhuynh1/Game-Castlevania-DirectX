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
	static CPlayScene* __instance;
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
	static CPlayScene* GetInstance();
	CPlayScene();
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	int GetId() { return this->id; }

	LPCWSTR  GetFilePath() { return this->sceneFilePath; };
   CPlayScene* GetInstanceOfPlayScene() {
		return this;
	}
	Simon * GetPlayer() { return player; } 

	friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	CPlayScene* p;
public: 
	int Access(CPlayScene& playscene)
	{
		return playscene.objects.size();
		
	}
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
 

};

