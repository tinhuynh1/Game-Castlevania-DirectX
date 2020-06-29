#pragma once
#include "GameObject.h"
class Effect : public CGameObject
{
public:
	Effect();
	~Effect();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	DWORD timer;

};

