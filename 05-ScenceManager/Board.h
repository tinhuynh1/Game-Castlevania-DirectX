#pragma once
#include"Game.h"
#include"GameObject.h"
#include "Code.h"
#include "Simon.h"
#define NUM_TIME 300
#define NUM_COUNT_TIME 1000
class Board
{
	//int score;
	int time;
	//Simon* simon;
	static Board* __instance;
	
	Code* code;
	DWORD count;
	DWORD timecount;
	int timeBoard;
	bool isStop = false;
	int healthSimon;
	int numLifeBoard;
	int scoreHeartBoard;
public:
	Board();
	void Render();
	void Update(DWORD dt);
	static Board* GetInstance();
	//get
	int GetTimeBoard() { return timeBoard; }
	int GetScoreHeartBoard() { return scoreHeartBoard; }
	//set
	void SetTimeHud(int timeBoard) { this->timeBoard = timeBoard; }
};
typedef Board* LPBOARD;