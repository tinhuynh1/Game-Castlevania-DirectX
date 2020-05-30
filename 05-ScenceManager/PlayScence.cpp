﻿#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;
//CPlayScene::CPlayScene() : CScene(this->id, this->sceneFilePath)
//{
//	
//	key_handler = new CPlayScenceKeyHandler(this);
//}
CPlayScene::CPlayScene(int id, LPCWSTR filePath):CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}


/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP_INFO				7
#define SCENE_SECTION_MAPS 8

#define OBJECT_TYPE_SIMON						0
#define OBJECT_TYPE_BRICK						1	 
#define OBJECT_TYPE_TORCH					2	 
#define OBJECT_TYPE_WHIP						3	 
#define OBJECT_TYPE_ITEM_HEART		4	 
#define OBJECT_TYPE_ITEM_CHAIN			5	 
#define OBJECT_TYPE_ITEM_DAGGER			6	 
#define OBJECT_TYPE_DAGGER					7	 
#define OBJECT_TYPE_KNIGHT		8 
#define OBJECT_TYPE_BOTSTAIR	9
#define OBJECT_TYPE_TOPSTAIR	10

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		//obj = new Simon(x,y); 
		obj = Simon::GetInstance();
		player = (Simon*)obj;  
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BRICK: 
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		obj = new CBrick();
		obj->SetWidth(width);
		obj->SetHeight(height);
		break;
	}
	case OBJECT_TYPE_WHIP: obj = new Whip(); break;
	case OBJECT_TYPE_KNIGHT: obj = new Knight(); break;
	case OBJECT_TYPE_BOTSTAIR:
	{
		obj = new BotStair(); 
		int direction = atoi(tokens[5].c_str());
		DebugOut(L"Bot stair direction is: %d  \n", direction);
		if(direction == 1)
		obj->StairTag = CGameObject::StairTypes::ToRight;
		else
		obj->StairTag = CGameObject::StairTypes::ToLeft;
		break;
	}
	case OBJECT_TYPE_TOPSTAIR:
	{
		obj = new TopStair();
		int direction = atoi(tokens[5].c_str());
		DebugOut(L"Top stair direction is: %d  \n", direction);
		if (direction == 1)
		obj->StairTag = CGameObject::StairTypes::ToRight;
		else		
		obj->StairTag = CGameObject::StairTypes::ToLeft;

		break;
	}
	case OBJECT_TYPE_TORCH: 
	{
		int i = atoi(tokens[4].c_str());
		int state = atoi(tokens[5].c_str());
		obj = new Torch();
		obj->SetState(state);
		obj->SetItemId(i);
		break;
	}
	case OBJECT_TYPE_ITEM_HEART: 
	{
		obj = new HeartItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_HEART, obj);
		break;
	}
	case OBJECT_TYPE_ITEM_CHAIN:
	{
		obj = new ChainItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_CHAIN, obj);
		break;
	}
	case OBJECT_TYPE_ITEM_DAGGER:
	{
		obj = new DaggerItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_DAGGER, obj);
		break;
	}
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}
void CPlayScene::_ParseSection_MAP_INFO(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 2) return; // skip invalid lines
	this->tileColumns = atoi(tokens[0].c_str());
	this->tileRows = atoi(tokens[1].c_str());
}
void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 4) return; // skip invalid lines - an object set must have at least id, x, y
	for (int i = 0; i < tokens.size(); i++)
	{
		RECT rectTile;
		int index = atoi(tokens[i].c_str());
		rectTile.left = (index % tileColumns) * TILE_WIDTH;
		rectTile.top = (index / tileColumns) * TILE_HEIGHT;
		rectTile.right = rectTile.left + TILE_WIDTH;
		rectTile.bottom = rectTile.top + TILE_HEIGHT;
		int x, y;
		x = i * TILE_WIDTH;
		y = this->tileMapLineY;
		Map* map = new Map(x, y, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);
		tileMap.push_back(map);
	}
	this->tileMapLineY += TILE_HEIGHT;
}
void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[MAP_INFO]") {
			section = SCENE_SECTION_MAP_INFO; continue;}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAPS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_MAP_INFO: _ParseSection_MAP_INFO(line); break;
			case SCENE_SECTION_MAPS:	_ParseSection_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 0, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	DebugOut(L"[INFO] Size is: %d", this->objects.size());
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->visible == false)
			continue;
		/*if (dynamic_cast<BotStair*>(objects[i]) || dynamic_cast<TopStair*>(objects[i]))
		{
			continue;
		}*/
		/*else if (dynamic_cast<Torch*>(objects[i]))
		{
			continue;
		}*/
		else
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->visible == false)
			continue;
		objects[i]->Update(dt, &coObjects);
	}
	listStair.clear();
	listTorch.clear();
	listItem.clear();
	listPortal.clear();
	for (UINT i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT temp = objects.at(i);
		if (dynamic_cast<BotStair*>(temp) || dynamic_cast<TopStair*>(temp))
		{
			listStair.push_back(objects.at(i));
		}
		else if(dynamic_cast<Torch*>(temp))
		{
			listTorch.push_back(objects.at(i));
		}
		/*else if (dynamic_cast<Torch*>(temp))
		{
			listTorch.push_back(objects.at(i));
		}*/
		else if (dynamic_cast<HeartItem*>(temp) || dynamic_cast<ChainItem*>(temp) || dynamic_cast<DaggerItem*>(temp))
		{
			listItem.push_back(objects.at(i));
		}
		else if (dynamic_cast<CPortal*>(temp))
		{
			listPortal.push_back(objects.at(i));
		}
	}
	CheckCollision_ItemAndSimon();
	CheckCollision_TorchAndSimon();
	CheckCollision_PortalAndSimon();
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow simon
	float cx, cy;
	player->GetPosition(cx, cy); //50.0f, 0.0f
	if(cx <= 0)
	{
		player->x = 0;
	}
	CGame *game = CGame::GetInstance();
	if ((cx > game->GetScreenWidth() / 2))
	{
		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;
		if (cx > 480)
		{
			return;
		}
	}

	else
	{
		cx = 0.0f;
	}
	CGame::GetInstance()->SetCamPos(cx, -10.0f /*cy*/);
}

void CPlayScene::Render()
{
	for (int i = 0; i < tileMap.size(); i++)
		tileMap[i]->Render();
	for (int i = objects.size() - 1; i >= 0; i--)
	{
		if (objects[i]->visible == false)
			continue;
		objects[i]->Render();
	}
	//CGame* game = CGame::GetInstance();
}
/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Simon*>(objects[i]) || dynamic_cast<Whip*>(objects[i]))
		{
			;
		}
		else
		{
			delete objects[i];
		}
	}
	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
	tileMap.clear();
}
void CPlayScene::CheckCollision_ItemAndSimon()
{
	for (UINT i = 0; i < listItem.size(); i++)
	{
		if (listItem.at(i)->visible == true)
		{
			if (player->CheckCollision(listItem.at(i)))
			{
				if (dynamic_cast<HeartItem*>(listItem.at(i)))
				{
					listItem.at(i)->visible = false;
				}
			}
			if (player->CheckCollision(listItem.at(i)))
			{
				if (dynamic_cast<ChainItem*>(listItem.at(i)))
				{
					player->isEatingItem = true;
					listItem.at(i)->visible = false;
					player->whip->LevelUp();
				}
			}
			if (player->CheckCollision(listItem.at(i)))
			{
				if (dynamic_cast<DaggerItem*>(listItem.at(i)))
				{
					player->isCollectDagger = true;
					player->isEatingItem = true;
					listItem.at(i)->visible = false;
				}
			}
		}
	}
}
void CPlayScene::CheckCollision_TorchAndSimon()
{
	for (UINT i = 0; i < listTorch.size(); i++)
	{
		if (player->CheckCollision(listTorch.at(i)))
		{
			//player->x += player->dx;
		}
	}
}
void CPlayScene::CheckCollision_PortalAndSimon()
{
	for (UINT i = 0; i < listPortal.size(); i++)
	{
		if (player->CheckCollision(listPortal.at(i)))
		{
			CGame::GetInstance()->SwitchScene(2);
		}
	}
}
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon *simon = ((CPlayScene*)scence)->GetPlayer();
	
	if (simon->isEatingItem)
	{
		simon->SetState(SIMON_STATE_IDLE);
		return;
	}
	switch (KeyCode)
	{
	case DIK_S:
	{
		if ((simon->GetState() == SIMON_STATE_ATTACK) || (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK))
			return;
		if (simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_JUMP)
		{			
			simon->SetState(SIMON_STATE_ATTACK);		
		}
		
		else if (simon->GetState() == SIMON_STATE_SIT) {
			simon->isAttack = true;
			simon->SetState(SIMON_STATE_SIT_AND_ATTACK);
		}
		break;
	}
	case DIK_X:
	{
		if (simon->isCollectDagger)
		{
			DebugOut(L" Dagger Available\n");
			simon->SetState(SIMON_STATE_THROW);
		}
		else
		{
			DebugOut(L"NOT OK \n");
		}
		break;
	}
	case DIK_SPACE:
	{
		if (simon->isAttack) return;
		if (simon->isOnStair) return;
		if (simon->isJumping == false)
		{
			simon->SetState(SIMON_STATE_JUMP);
			simon->isJumping = true;
		}
		break;
	}
	case DIK_A: 
		simon->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	Simon* simon = ((CPlayScene*)scence)->GetPlayer();
	vector<LPGAMEOBJECT> listStair = ((CPlayScene*)scence)->GetListStair();
	if (simon->isEatingItem)
	{
		simon->SetState(SIMON_STATE_IDLE);
		return;
	}
	if (simon->GetState() == SIMON_STATE_JUMP) return;
	if (simon->GetState() == SIMON_STATE_ATTACK && simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
		return;
	if (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK && simon->animation_set->at(SIMON_ANI_SIT_AND_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
		return;
	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))		//simon->SetState(SIMON_STATE_WALKING_RIGHT);
	{						
		if (simon->GetState() == SIMON_STATE_ATTACK || simon->GetState() == SIMON_STATE_SIT_AND_ATTACK) return;
		if (game->IsKeyDown(DIK_DOWN))
		{
			if (simon->isOnStair == false)
			{
				simon->SetState(SIMON_STATE_SIT);
				return;
			}
		}
		for (int i = 0; i < listStair.size(); i++)
		{
			LPGAMEOBJECT temp = listStair.at(i);
			if (dynamic_cast<BotStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(listStair.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitBottomStair = true;
					break;
				}
			}
			else
			{
				if (CGame::GetInstance()->AABB(listStair.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitTopStair = true;
					break;
				}
			}
		}
		if (simon->isOnStair)
		{
			DebugOut(L"OK------------\n");
			if (simon->isUpstair)
				simon->isHitBottomStair = false;
			if (simon->isUpstair == false)
				simon->isHitTopStair = false;

			if (simon->StairDirection == 1)
			{
				simon->isUpstair = true;
			}
			else if (simon->StairDirection == -1)
			{
				if (simon->isUpstair)
				{
					simon->isUpstair = false;
				}
				else
				{
					simon->isUpstair = false;
				}
			}
			simon->isStopOnStair = false;
			simon->SetState(SIMON_STATE_ONSTAIR);
			return;
		}
		else
		{
			simon->SetOrientation(1);
			simon->SetState(SIMON_STATE_WALKING);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (simon->isAttack || simon->isSitAttack) return;
		if (game->IsKeyDown(DIK_DOWN))
		{
			if (simon->isOnStair == false)
			{
				simon->SetState(SIMON_STATE_SIT);
				return;
			}
		}
		for (int i = 0; i < listStair.size(); i++)
		{
			LPGAMEOBJECT temp = listStair.at(i);
			if (dynamic_cast<BotStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(listStair.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitBottomStair = true;
					break;
				}
			}
			else
			{
				if (CGame::GetInstance()->AABB(listStair.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitTopStair = true;
					break;
				}
			}
		}
		if (simon->isOnStair)
		{
			// dòng này để tránh Simon đang lên cầu thang nhưng lại checkColWithStair do biến isHitBottom vẫn true
				if (simon->isUpstair)
					simon->isHitBottomStair = false;
			if (simon->isUpstair == false)
				simon->isHitTopStair = false;

			if (simon->StairDirection == 1)  //stair từ trái qua phải
			{
				simon->isUpstair = true;
			}
			else if (simon->StairDirection == -1)  //stair từ phải qua trái
			{
				if (simon->isUpstair)
				{
					simon->isUpstair = false;
				}
				else
				{
					simon->isUpstair = false;
				}
			}
			simon->isStopOnStair = false;
			simon->SetState(SIMON_STATE_ONSTAIR);
			return;
		}
		else
		{
			simon->SetOrientation(-1);
			simon->SetState(SIMON_STATE_WALKING);
		}
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->isAttack) return;
		for (int i = 0; i < listStair.size(); i++)
		{
			LPGAMEOBJECT temp = listStair.at(i);
			if (dynamic_cast<BotStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(listStair.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitBottomStair = true;
				}
			}
			else if (dynamic_cast<TopStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(listStair.at(i)->GetBound(), simon->GetBound()))
				{
					if (simon->nx > 0)
					{
						simon->StairDirection = -1;
						if (listStair.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
						{
							simon->StairDirection = 1;
						}
					}
					else if (simon->nx < 0)
					{
						simon->StairDirection = 1;
						if (listStair.at(i)->StairTag == CGameObject::StairTypes::ToRight)
						{
							simon->StairDirection = -1;
						}
					}
					if (abs(simon->x - listStair.at(i)->x) <= 10)
					{
						DebugOut(L"Distance is: %f", (abs(simon->x - listStair.at(i)->x)));
						if (listStair.at(i)->StairTag == CGameObject::StairTypes::ToRight)
						{
							DebugOut(L"To Right \n");
							simon->isHitTop = true;
							simon->isWalkingToStair = true;
							simon->nx = 1;
							simon->SetState(SIMON_STATE_WALKING);
						}
						else if (listStair.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
						{
							DebugOut(L"To Left \n");
							simon->isHitTop = true;
							simon->isWalkingToStair = true;
							simon->nx = -1;
							simon->SetState(SIMON_STATE_WALKING);
						}
					}
					else
					{
						simon->isOnStair = true;
					}
					if (listStair.at(i)->StairTag == CGameObject::StairTypes::ToRight)
					{
						simon->isHitTop = true;
						simon->SetPosition(listStair.at(i)->x-10, simon->GetPosition().y);
						simon->isOnStair = true;
					}
				}
			}
		}
		if (simon->isOnStair)
		{
			simon->isHitTop = false;
			simon->isStopOnStair = false;
			if (simon->isUpstair)
				simon->isHitBottomStair = false;
			simon->isUpstair = false;
			simon->SetState(SIMON_STATE_ONSTAIR);
		}
		else
		{
			if (simon->isHitTop == false)
			{
				if (simon->isAttack == false && simon->isJumping == false)
					simon->SetState(SIMON_STATE_SIT);
			}
		}
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		if (simon->state == SIMON_STATE_JUMP) return;
		if (simon->isAttack) return; //khi đánh trên cầu thang thì k lên		
		for (int i = 0; i < listStair.size(); i++)
		{
			LPGAMEOBJECT temp = listStair.at(i);

			if (dynamic_cast<BotStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(listStair.at(i)->GetBound(), simon->GetBound()))
				{
					if (listStair.at(i)->StairTag == CGameObject::StairTypes::ToRight)
					{
						simon->StairDirection = 1;
					}
					else if (listStair.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
					{
						simon->StairDirection = -1;
					}
					if (abs(simon->x - listStair.at(i)->x) < 14)
					{
						//DebugOut(L"Distance is: %f \n", abs(simon->x - list.at(i)->x));
						if (listStair.at(i)->StairTag == CGameObject::StairTypes::ToRight)
						{
							simon->isWalkingToStair = true;
							simon->nx = 1;
							simon->SetState(SIMON_STATE_WALKING);
						}
						else if (listStair.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
						{
							simon->isWalkingToStair = true;
							simon->nx = -1;
							simon->SetState(SIMON_STATE_WALKING);
						}
					}
					else
					{
						simon->isWalkingToStair = false;
						simon->isOnStair = true;
						break;
					}
				}
			}
			else if (dynamic_cast<TopStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(listStair.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitTopStair = true;
				}
			}
		}
		if (simon->isOnStair)
		{
			simon->isStopOnStair = false;
			if (simon->isUpstair == false) //dòng này để tránh Simon đang xuống cầu thang nhưng lại checkColWithStair do biến isHitTopStair vẫn true
				simon->isHitTopStair = false;
			simon->isUpstair = true;
			simon->SetState(SIMON_STATE_ONSTAIR);
		}
	}
	/*else
		simon->SetState(SIMON_STATE_IDLE);*/
}

void  CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	Simon* simon = ((CPlayScene*)scence)->GetPlayer();
	if (simon->GetState() == SIMON_STATE_DIE) return;
	switch (KeyCode)
	{
	case DIK_LEFT:
		if (simon->isOnStair)
		{
			simon->isStopOnStair = true;
			simon->vy = 0;
			simon->vx = 0;
		}
		else
		{
			if((simon->isAttack) ||(simon->state == SIMON_STATE_SIT))
				return;
			simon->SetState(SIMON_STATE_IDLE);
		}
		break;
	case DIK_RIGHT:
		if (simon->isOnStair)
		{
			simon->isStopOnStair = true;
			simon->vx = 0;
			simon->vy = 0;
		}
		else
		{
			if ((simon->isAttack) || (simon->state == SIMON_STATE_SIT))
				return;
			simon->SetState(SIMON_STATE_IDLE);
		}
		break;
	case DIK_DOWN:
		if (simon->isOnStair)
		{
			simon->isStopOnStair = true;
			simon->vx = 0;
			simon->vy = 0;
		}
		else
		{
			if ((simon->isAttack) || (simon->isSitAttack))
				return;
			simon->SetState(SIMON_STATE_IDLE);
		}
		break;
	case DIK_UP:
	{
		
		if (simon->isUpstair == true)
		{
			simon->isStopOnStair = true;
			simon->vx = 0;
			simon->vy = 0;
		}
		break;
	}
	}

}
