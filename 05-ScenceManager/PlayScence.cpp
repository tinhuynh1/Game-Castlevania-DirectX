#include <iostream>
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
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_TORCH   4
#define OBJECT_TYPE_WHIP   5
#define OBJECT_TYPE_ITEM_HEART		6
#define OBJECT_TYPE_ITEM_CHAIN		7
#define OBJECT_TYPE_ITEM_DAGGER		8
#define OBJECT_TYPE_BOTSTAIR   9
#define OBJECT_TYPE_TOPSTAIR   10
#define SCENE_SECTION_MAPS 7
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());
	if (texID == ID_TILE_MAP) {
		this->tileColumns = atoi(tokens[5].c_str());
		this->tileRows = atoi(tokens[6].c_str());
	}

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
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
		obj = new Simon(x,y); 
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
	case OBJECT_TYPE_BOTSTAIR:
	{
		obj = new BotStair(); 
		obj->StairTag = CGameObject::StairTypes::ToRight;
		break;
	}
	case OBJECT_TYPE_TOPSTAIR:
	{
		obj = new TopStair();
		obj->StairTag = CGameObject::StairTypes::ToLeft;
		break;
	}
	case OBJECT_TYPE_TORCH: 
	{
		int i = atoi(tokens[4].c_str());
		obj = new Torch();
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

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAPS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
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
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->visible == false)
			continue;
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->visible == false)
			continue;
		objects[i]->Update(dt, &coObjects);
	}
	//if (CGame::GetInstance()->IsKeyDown(DIK_DOWN))
	//{
	//	if ((player->isAttack)) return;
	//	for (int i = 0; i < objects.size(); i++)
	//	{
	//		LPGAMEOBJECT temp = objects.at(i);
	//		if (dynamic_cast<BotStair*>(temp))
	//		{ 
	//			if (CGame::GetInstance()->AABB(objects.at(i)->GetBound(), player->GetBound()))
	//			{
	//				player->isHitBottomStair = true;
	//			}
	//		}
	//		else if (dynamic_cast<TopStair*>(temp))
	//		{
	//			if (CGame::GetInstance()->AABB(objects.at(i)->GetBound(), player->GetBound()))
	//			{
	//				if (player->nx > 0)
	//				{
	//					player->StairDirection = -1;
	//					if (objects.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
	//					{
	//						player->StairDirection = 1;
	//					}
	//				}
	//				else if (player->nx < 0)
	//				{
	//					player->StairDirection = 1;
	//					if (objects.at(i)->StairTag == CGameObject::StairTypes::ToRight)
	//					{
	//						player->StairDirection = -1;
	//					}
	//				}
	//				if (abs(player->x - objects.at(i)->x) <= 10)
	//				{
	//					if(objects.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
	//					{
	//						player->isHitTop = true;
	//						player->isWalkingToStair = true;
	//						player->nx = 1;
	//						player->SetState(SIMON_STATE_WALKING);
	//					}
	//				}
	//				else
	//				{
	//					player->isOnStair = true;
	//				}
	//				if (objects.at(i)->StairTag == CGameObject::StairTypes::ToRight)
	//				{
	//					player->isHitTop = true;
	//					player->SetPosition(objects.at(i)->x, player->GetPosition().y);
	//					player->isOnStair = true;
	//				}
	//			}
	//		}
	//		if (player->isOnStair)
	//		{
	//			player->isStopOnStair = false;
	//			if (player->isUpstair == false) // dòng này để tránh simon đang xuống cầu thang nhưng lại checkColWithStair do biến isHitTopStair vẫn true
	//				player->isHitTopStair == false;
	//			player->isUpstair = true;
	//			player->SetState(SIMON_STATE_ONSTAIR);
	//		}
	//	}
	//	
	//}
	/*for (int i = 0; i < objects.size(); i++)
		{
			LPGAMEOBJECT temp = objects.at(i);
			if (dynamic_cast<BotStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(objects.at(i)->GetBound(), player->GetBound()))
				{
					if (objects.at(i)->StairTag == CGameObject::StairTypes::ToRight)
						player->StairDirection = 1;
					else if (objects.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
						player->StairDirection = -1;
					if (abs(player->x - objects.at(i)->x) < 14)
					{
						if (objects.at(i)->StairTag == CGameObject::StairTypes::ToRight)
						{
							player->isWalkingToStair = true;
							player->nx = 1;
							player->SetState(SIMON_STATE_WALKING);
							
							DebugOut(L"Vx: true \n");
						}
						else if (objects.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
						{
							player->isWalkingToStair = true;
							player->nx = -1;
							player->SetState(SIMON_STATE_WALKING);
						}
					}
					else
					{
						player ->isWalkingToStair = false;
						player->isOnStair = true;
						break;
					}
				}
			}
			else if (dynamic_cast<TopStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(objects.at(i)->GetBound(), player->GetBound()))
				{
					player->isHitTopStair = true;
				}
			}

		}*/
	
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
		if (cx > 450)
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
			DebugOut(L"Object [%d] \n", objects.size());
		}
		else
		{
			delete objects[i];
		}
	}

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon *simon = ((CPlayScene*)scence)->GetPlayer();
	

	switch (KeyCode)
	{
	case DIK_S:
	{
		if ((simon->GetState() == SIMON_STATE_ATTACK) || (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK))
			return;
		if (simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_JUMP)
			simon->SetState(SIMON_STATE_ATTACK);
		else if (simon->GetState() == SIMON_STATE_SIT) {
			simon->SetState(SIMON_STATE_SIT_AND_ATTACK);
		}
		break;
	}
	case DIK_SPACE:
	{
		if (simon->GetState() == SIMON_STATE_JUMP || simon->GetState()==SIMON_STATE_ATTACK || simon->GetState() == SIMON_STATE_SIT_AND_ATTACK) return;
		simon->SetState(SIMON_STATE_JUMP);
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
	vector<LPGAMEOBJECT> list = ((CPlayScene*)scence)->GetObjects();
	if (simon->GetState() == SIMON_STATE_JUMP) return;
	if (simon->GetState() == SIMON_STATE_ATTACK && simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
		return;
	if (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK && simon->animation_set->at(SIMON_ANI_SIT_AND_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
		return;
	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))		//simon->SetState(SIMON_STATE_WALKING_RIGHT);
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
		for (int i = 0; i < list.size(); i++)
		{
			LPGAMEOBJECT temp = list.at(i);
			if (dynamic_cast<BotStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(list.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitBottomStair = true;
					break;
				}
			}
			else
			{
				if (CGame::GetInstance()->AABB(list.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitTopStair = true;
					break;
				}
			}
		}
		if (simon->isOnStair)
		{
			if (simon->isUpstair)
				simon->isHitBottomStair = false;
			if (simon->isUpstair == false)
				simon->isHitTopStair = false;
			if (simon->StairDirection == 1)
			{
				simon->isUpstair = true;
			}
			else if (simon->StairDirection = -1)
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
		for (int i = 0; i < list.size(); i++)
		{
			LPGAMEOBJECT temp = list.at(i);
			if (dynamic_cast<BotStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(list.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitBottomStair = true;
					break;
				}
			}
			else
			{
				if (CGame::GetInstance()->AABB(list.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitTopStair = true;
					break;
				}
			}
		}
		if (simon->isOnStair)
		{
			if (simon->isUpstair)
				simon->isHitBottomStair = false;
			if (simon->isUpstair == false)
				simon->isHitTopStair = false;
			if (simon->StairDirection == 1)
			{
				if (simon->isUpstair)
				{
					simon->isUpstair = false;
				}
				else
				{
					if (simon->nx < 0)
					{
						simon->isUpstair = false;
					}
					else
					{
						simon->isUpstair = true;
					}
				}
			}
			else if (simon->StairDirection == -1)
			{
				simon->isUpstair = true;
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
		for (int i = 0; i < list.size(); i++)
		{
			LPGAMEOBJECT temp = list.at(i);
			if (dynamic_cast<BotStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(list.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitBottomStair = true;
				}
			}
			else if (dynamic_cast<TopStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(list.at(i)->GetBound(), simon->GetBound()))
				{
					if (simon->nx > 0)
					{
						simon->StairDirection = -1;
						if (list.at(i)->AABB(list.at(i)->GetBound(), simon->GetBound()))
						{
							if (simon->nx > 0)
							{
								simon->StairDirection = -1;
								if (list.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
								{
									simon->StairDirection = 1;
								}
							}
							else if (simon->nx < 0)
							{
								simon->StairDirection = 1;
								if (list.at(i)->StairTag == CGameObject::StairTypes::ToRight)
								{
									simon->StairDirection = -1;
								}
							}
							if (abs(simon->x - list.at(i)->x) < 10)
							{
								if (list.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
								{
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
							if (list.at(i)->StairTag == CGameObject::StairTypes::ToRight)
							{
								simon->isHitTop = true;
								simon->SetPosition(list.at(i)->x, simon->GetPosition().y);
								simon->isOnStair = true;
							}
						}
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
		for (int i = 0; i < list.size(); i++)
		{
			LPGAMEOBJECT temp = list.at(i);

			if (dynamic_cast<BotStair*>(temp))
			{
				if (CGame::GetInstance()->AABB(list.at(i)->GetBound(), simon->GetBound()))
				{
					if (list.at(i)->StairTag == CGameObject::StairTypes::ToRight)
					{
						simon->StairDirection = 1;
					}
					else if (list.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
					{
						simon->StairDirection = -1;
					}
					if (abs(simon->x - list.at(i)->x) < 14)
					{
						if (list.at(i)->StairTag == CGameObject::StairTypes::ToRight)
						{
							simon->isWalkingToStair = true;
							simon->nx = 1;
							simon->SetState(SIMON_STATE_WALKING);
						}
						else if (list.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
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
				if (CGame::GetInstance()->AABB(list.at(i)->GetBound(), simon->GetBound()))
				{
					simon->isHitTopStair = true;
				}
			}
		}
		if (!simon->isOnStair)
		{
			simon->isStopOnStair = false;
			if (simon->isUpstair == false) //dòng này để tránh Simon đang xuống cầu thang nhưng lại checkColWithStair do biến isHitTopStair vẫn true
				simon->isHitTopStair = false;
			simon->isUpstair = true;
			simon->SetState(SIMON_STATE_ONSTAIR);
		}
	}
	else
		simon->SetState(SIMON_STATE_IDLE);
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
		DebugOut(L"Key Up \n");
		simon->isStopOnStair = true;
		simon->vx = 0;
		simon->vy = 0;
	}

}
