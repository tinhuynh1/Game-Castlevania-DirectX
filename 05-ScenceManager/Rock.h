#pragma once
#include "GameObject.h"
#include "Utils.h"
class Rock : public CGameObject
{
public:
	Rock();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
};
class Rocks
{
public:
	vector <Rock*> rocks;
	static Rocks* __instance;
	void AddRock(Rock* rock);
	void DropRock(float x, float y);
	Rocks() {};
	static Rocks* GetInstance();
};
