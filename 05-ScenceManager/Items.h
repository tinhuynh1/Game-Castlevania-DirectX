#pragma once
#include"GameObject.h"
class Items
{
	unordered_map<int, vector<LPGAMEOBJECT>> items;
	static Items* __instance;
public:
	void AddItem(int itemType, LPGAMEOBJECT item);
	void CheckAndDrop(LPGAMEOBJECT object);
	void Drop(int itemType, float x, float y);
	Items() {};
	static Items* GetInstance();

};

