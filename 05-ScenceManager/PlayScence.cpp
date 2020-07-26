#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;
CPlayScene::CPlayScene(int id, LPCWSTR filePath):CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}


/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TILE_SHEET 1
#define SCENE_SECTION_SPRITES 2
#define SCENE_SECTION_ANIMATIONS 3
#define SCENE_SECTION_ANIMATION_SETS 4
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP_INFO				7
#define SCENE_SECTION_MAPS 8

#define MAX_SCENE_LINE 1024


/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_TILE_SHEET(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());
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
		int frame_time = atoi(tokens[i + 1].c_str());
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

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_BRICK:
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		obj = new CBrick();
		obj->SetWidth(width);
		obj->SetHeight(height);
		listBrick.push_back(obj);
		for (int i = 6; i < tokens.size(); i+=2)
		{
			int rowIndex = atoi(tokens[i].c_str());
			int colIndex = atoi(tokens[i+1].c_str());
			(grid->cells[rowIndex][colIndex]).push_back(obj);
		}
		break;
	}
	case OBJECT_TYPE_SIMON:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = Simon::GetInstance();
		player = (Simon*)obj;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_CROWN_ITEM:
	{
		obj = new CrownItem();
		//Items::GetInstance()->AddItem(OBJECT_TYPE_CROWN_ITEM, obj);
		listItem.push_back(obj);
		break;
	}
	case OBJECT_TYPE_WHIP: obj = new Whip(); break;
	case OBJECT_TYPE_BAT:
	{
		obj = new Bat(x, y);
		listEnemy.push_back(obj);
		break;
	}
	case OBJECT_TYPE_DAGGER:
	{
		obj = new Dagger();
		dagger = (Dagger*)obj;
		obj->visible = false;
		listWeapon.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BOOMERANG:
	{
		obj = new Boomerang();
		boomerang = (Boomerang*)obj;
		obj->visible = false;
		listWeapon.push_back(obj);
		break;
	}
	case OBJECT_TYPE_HOLYWATER:
	{
		obj = new HolyWater();
		holywater = (HolyWater*)obj;
		obj->visible = false;
		listWeapon.push_back(obj);
		break;
	}
	case OBJECT_TYPE_AXE:
	{
		obj = new Axe();
		axe = (Axe*)obj;
		obj->visible = false;
		listWeapon.push_back(obj);
		break;
	}
	case OBJECT_TYPE_KNIGHT:
	{
		obj = new Knight(x, y);
		listEnemy.push_back(obj);
		break;
	}
	case OBJECT_TYPE_GHOUL:
	{
		int direction = atoi(tokens[4].c_str());
		obj = new Ghoul();
		obj->SetOrientation(direction);
		listEnemy.push_back(obj);
		break;
	}
	case OBJECT_TYPE_GHOST:
	{
		int direction = atoi(tokens[4].c_str());
		obj = new Ghost(player);
		obj->SetOrientation(direction);
		listEnemy.push_back(obj);
		break;
	}
	case OBJECT_TYPE_FLEAMAN:
	{
		int direction = atoi(tokens[4].c_str());
		obj = new Fleaman(player);
		obj->SetOrientation(direction);
		listEnemy.push_back(obj);
		break;
	}
	case OBJECT_TYPE_RAVEN:
	{
		int direction = atoi(tokens[4].c_str());
		obj = new Raven();
		obj->SetOrientation(direction);
		listEnemy.push_back(obj);
		break;
	}
	case OBJECT_TYPE_SKELETON:
	{
		//int direction = atoi(tokens[4].c_str());
		obj = new Skeleton(player);
		//obj->SetOrientation(direction);
		listEnemy.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BOSS:
		obj = new Boss(player);
		listEnemy.push_back(obj);
		break;
	case OBJECT_TYPE_ROCKS: 
		obj = new Rock();
		Rocks::GetInstance()->AddRock((Rock*)obj);
		listEfect.push_back(obj);
		break;
	case OBJECT_TYPE_MOVING_PFLATFORM: 
	{
		obj = new MovingPlatform();
		for (int i = 4; i < tokens.size(); i += 2)
		{
			int rowIndex = atoi(tokens[i].c_str());
			int colIndex = atoi(tokens[i + 1].c_str());
			(grid->cells[rowIndex][colIndex]).push_back(obj);
		}
		listBrick.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BOTSTAIR:
	{
		obj = new BotStair(); 
		int direction = atoi(tokens[3].c_str());
		if(direction == 1)
		obj->StairTag = CGameObject::StairTypes::ToRight;
		else
		obj->StairTag = CGameObject::StairTypes::ToLeft;
		for (int i = 4; i < tokens.size(); i += 2)
		{
			int rowIndex = atoi(tokens[i].c_str());
			int colIndex = atoi(tokens[i + 1].c_str());
			(grid->cells[rowIndex][colIndex]).push_back(obj);
		}
		listTorch.push_back(obj);
		listStair.push_back(obj);
		break;
	}
	case OBJECT_TYPE_TOPSTAIR:
	{
		obj = new TopStair();
		int direction = atoi(tokens[3].c_str());
		if (direction == 1)
		obj->StairTag = CGameObject::StairTypes::ToRight;
		else		
		obj->StairTag = CGameObject::StairTypes::ToLeft;
		for (int i = 4; i < tokens.size(); i += 2)
		{
			int rowIndex = atoi(tokens[i].c_str());
			int colIndex = atoi(tokens[i + 1].c_str());
			(grid->cells[rowIndex][colIndex]).push_back(obj);
		}
		listTorch.push_back(obj);
		listStair.push_back(obj);
		break;
	}
	case OBJECT_TYPE_TORCH: 
	{
		int i = atoi(tokens[4].c_str());
		int state = atoi(tokens[5].c_str());
		obj = new Torch();
		obj->SetState(state);
		obj->SetItemId(i);
		for (int i = 6; i < tokens.size(); i += 2)
		{
			int rowIndex = atoi(tokens[i].c_str());
			int colIndex = atoi(tokens[i + 1].c_str());
			(grid->cells[rowIndex][colIndex]).push_back(obj);
		}
		listTorch.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BREAKABLE_BRICK:
	{
		obj = new BreakableBrick();
		for (int i = 4; i < tokens.size(); i += 2)
		{
			int rowIndex = atoi(tokens[i].c_str());
			int colIndex = atoi(tokens[i + 1].c_str());
			(grid->cells[rowIndex][colIndex]).push_back(obj);
		}
		listEnemy.push_back(obj);
		listBrick.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ITEM_HEART: 
	{
		obj = new HeartItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_HEART, obj);
		listItem.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ITEM_SMALL_HEART:
	{
		obj = new SmallHeartItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_SMALL_HEART, obj);
		listItem.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ITEM_MONEY_BAG:
	{
		obj = new MoneyBagItem();
		//obj->SetState(state);
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_MONEY_BAG, obj);
		listItem.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ITEM_CHAIN:
	{
		obj = new ChainItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_CHAIN, obj);
		listItem.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ITEM_DAGGER:
	{
		obj = new DaggerItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_DAGGER, obj);
		listItem.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ITEM_BOOMERANG:
	{
		obj = new BoomerangItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_BOOMERANG, obj);
		listItem.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ITEM_HOLYWATER:
	{
		obj = new HolyWaterItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_HOLYWATER, obj);
		listItem.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ITEM_AXE:
	{
		obj = new AxeItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_AXE, obj);
		listItem.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ITEM_WATCH:
	{
		obj = new WatchItem();
		Items::GetInstance()->AddItem(OBJECT_TYPE_ITEM_WATCH, obj);
		listItem.push_back(obj);
		break;
	}
	case OBJECT_TYPE_EFFECT:
	{
		obj = new Effect();
		listEffet1.push_back(obj);
		break;
	}
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
			listPortal.push_back(obj);
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
	//objects.push_back(obj);
}
void CPlayScene::_ParseSection_MAP_INFO(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 2) return; // skip invalid lines
	this->tileColumns = atoi(tokens[0].c_str());
	this->tileRows = atoi(tokens[1].c_str());
	this->map_width = atoi(tokens[2].c_str());
}
void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 4) return; // skip invalid lines - an object set must have at least id, x, y
	for (int i = 0; i < tokens.size(); i++)
	{
		RECT rectTile;
		int index = atoi(tokens[i].c_str());
		if (index >= 0)
		{
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
	}
	this->tileMapLineY += TILE_HEIGHT;
}
void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	HUD = Board::GetInstance();
	grid = new Grid();
	{
		listBrick.clear();
		listTorch.clear();
		listPortal.clear();
		listEnemy.clear();
		listStair.clear();
		listItem.clear();
		ifstream f;
		f.open(sceneFilePath);

		// current resource section flag
		int section = SCENE_SECTION_UNKNOWN;

		char str[MAX_SCENE_LINE];
		while (f.getline(str, MAX_SCENE_LINE))
		{
			string line(str);

			if (line[0] == '#') continue;	// skip comment lines	
			if (line == "[TILESHEET]") {
				section = SCENE_SECTION_TILE_SHEET; continue;
			}
			if (line == "[SPRITES]") {
				section = SCENE_SECTION_SPRITES; continue;
			}
			if (line == "[ANIMATIONS]") {
				section = SCENE_SECTION_ANIMATIONS; continue;
			}
			if (line == "[ANIMATIONS_SETS]") {
				section = SCENE_SECTION_ANIMATION_SETS; continue;
			}
			if (line == "[OBJECTS]") {
				section = SCENE_SECTION_OBJECTS; continue;
			}
			if (line == "[MAP_INFO]") {
				section = SCENE_SECTION_MAP_INFO; continue;
			}
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
			case SCENE_SECTION_TILE_SHEET: _ParseSection_TILE_SHEET(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			}
		}

		f.close();

		CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 0, 255));
	}
}

void CPlayScene::Update(DWORD dt)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	grid->GetListCollisionFromGrid(listColObjects);
	listStair.clear();
	listTorch.clear();
	listBrick.clear();
	for (UINT i = 0; i < listColObjects.size(); i++)
	{
		LPGAMEOBJECT temp = listColObjects.at(i);
		if (dynamic_cast<Torch*>(temp))
			listTorch.push_back(listColObjects.at(i));
		else
			if (dynamic_cast<TopStair*>(temp) || dynamic_cast<BotStair*>(temp))
				listStair.push_back(listColObjects.at(i));
			else
				listBrick.push_back(listColObjects.at(i));
	}
	CheckCollision_ItemAndSimon();
	CheckCollision_WhipAndTorch();
	CheckCollision_WhipAndEnemy();
	CheckCollision_PortalAndSimon();
	CheckCollision_WeaponAndTorch();
	CheckCollision_SimonAndEnemy();
	CheckCollision_WeaponAndEnemy();
	for (UINT i = 0; i < listBrick.size(); i++)
	{
		listBrick[i]->Update(dt, &listBrick);
	}
	player->Update(dt, &listBrick);
	for (UINT i = 0; i < listTorch.size(); i++)
	{
		if (listTorch[i]->isVisible() == true)
			listTorch[i]->Update(dt, &listBrick);
	}
	for (UINT i = 0; i < listItem.size(); i++)
	{
		if (listItem[i]->isVisible() == true)
		{
			listItem[i]->Update(dt, &listBrick);
			if (listItem[i]->timeToItemVisible < 3000)
			{
				listItem[i]->timeToItemVisible += dt;
			}
			else
			{
				listItem[i]->SetVisible(false);
				listItem[i]->timeToItemVisible = 0;
			}
		}
	}
	for (UINT i = 0; i < listEfect.size(); i++)
	{
		if (listEfect[i]->isVisible() == true)
			listEfect[i]->Update(dt, &listBrick);
	}
	for (UINT i = 0; i < listEffet1.size(); i++)
	{
		if (listEffet1[i]->isVisible() == true)
		{
			listEffet1[i]->Update(dt, &listBrick);
		}
	}
	for (UINT i = 0; i < listWeapon.size(); i++)
	{
		if (listWeapon[i]->isVisible() == true)
		{
			listWeapon[i]->Update(dt, &listBrick);
		}
	}
	for (UINT i = 0; i < listEnemy.size(); i++)
	{ 
		LPGAMEOBJECT temp = listEnemy.at(i);
		if ((listEnemy.at(i)->GetPosition().x+listEnemy.at(i)->GetWidth()) >= CGame::GetInstance()->GetCamPosition().x && listEnemy.at(i)->GetPosition().x < CGame::GetInstance()->GetCamPosition().x+ SCREEN_WIDTH)
		{
			listEnemy.at(i)->SetOutOfCamera(false);
		}
		else
		{
			listEnemy.at(i)->SetOutOfCamera(true);
		}
		if (dynamic_cast<Ghost*>(temp) || dynamic_cast<Raven*>(temp))
		{
			temp->Update(dt, &listBrick);
		}
		else if (dynamic_cast<Skeleton*>(temp))
		{
			temp->Update(dt, &listBrick);
			if (temp->isCreateBone == true)
			{

				int ani_set_id = 106;

				bone = new Bone();
				bone->SetPosition(temp->x, temp->y);
				bone->SetAnimationSet(animation_sets->Get(ani_set_id));
				bone->SetOrientation(temp->nx);
				bone->firstx = temp->x;
				listEnemy.push_back(bone);
				temp->isCreateBone = false;
			}
		}
		else if (listEnemy[i]->isVisible() == true)
			listEnemy[i]->Update(dt, &listBrick);
		if (player->isUseStop)
		{		
			if (timeToEnemyStop < 5000)
			{
				listEnemy.at(i)->isStop = true;
				timeToEnemyStop += dt;
			}
			else
			{
				timeToEnemyStop = 0;
				player->isUseStop = false;		
			}
		}
		else
		{
			listEnemy.at(i)->isStop = false;
		}
	}
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow simon
	float cx, cy;
	player->GetPosition(cx, cy); //50.0f, 0.0f
	if(cx <= 0)
	{
		player->x = 0;
	}
	if (isStateBoss)
	{
		if (player->x <= 512)
		{
			player->x = 512;
		}
	}
	CGame *game = CGame::GetInstance();
	 if(cx >= game->GetScreenWidth() / 2)
	 {
		
		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;
		if (cx > (map_width - (game->GetScreenWidth())))
		{
			cx = map_width - (game->GetScreenWidth());
		}
		
	 }
	else if (cx < game->GetScreenWidth() / 2)
	 {
		 cx = 0.0f;
	 }
	 if (player->isRevive)
	 {
		 player->isRevive = false;

		 player->SetVisible(true);
		 player->SetState(SIMON_STATE_IDLE);
		 player->SetHealth(16);
		 player->numLife--;
		 HUD->SetTimeHud(300);
		 //NOTE chỉ mới xử lí ở scene 2, chưa xử lí các scene khác
		 if (game->GetSceneId() == 2)
			 player->SetPosition(32, 130);
	 }
	 if (game->GetSceneId() == 6)
	 {
		 if(player->x> map_width - (game->GetScreenWidth()/2))
		 {
			 isStateBoss = true;
		 }
	 }
	 HUD->Update(dt);
	 if (isStateBoss)
	 {
		 CGame::GetInstance()->SetCamPos(512, -40.0f /*cy*/);
	 }
	 else
	 {
		 CGame::GetInstance()->SetCamPos(cx, -40.0f /*cy*/);
	 }
}

void CPlayScene::Render()
{
		for (int i = 0; i < tileMap.size(); i++)
		{
			tileMap[i]->Render();
		}
		for (UINT i = 0; i < listPortal.size(); i++)
		{
			listPortal.at(i)->Render();
		}
		for (UINT i = 0; i < listEfect.size(); i++)
		{
			if (listEfect.at(i)->isVisible() == true)
				listEfect.at(i)->Render();
		}
		for (UINT i = 0; i < listWeapon.size(); i++)
		{
			if (listWeapon.at(i)->isVisible() == true)
				listWeapon.at(i)->Render();
		}
		for (UINT i = 0; i < listTorch.size(); i++)
		{
			listTorch.at(i)->Render();
		}
		for (UINT i = 0; i < listStair.size(); i++)
		{
			listStair.at(i)->Render();
		}
		for (UINT i = 0; i < listEnemy.size(); i++)
		{
			if(listEnemy.at(i)->isVisible() == true)
			listEnemy.at(i)->Render();
		}
		for (UINT i = 0; i < listEffet1.size(); i++)
		{
			if (listEffet1.at(i)->isVisible() == true)
				listEffet1.at(i)->Render();
		}
		for (UINT i = 0; i < listEfect.size(); i++)
		{
			if (listEfect.at(i)->isVisible() == true)
				listEfect.at(i)->Render();
		}
		
		//Vẽ gạch
		for (UINT i = 0; i < listBrick.size(); i++)
		{
				listBrick.at(i)->Render();
		}
		for (UINT i = 0; i < listItem.size(); i++)
		{
			if (listItem.at(i)->isVisible() == true)
				listItem.at(i)->Render();
		}
		player->Render();
		HUD->Render();
		//CGame* game = CGame::GetInstance();
	
}
/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	listTorch.clear();
	listBrick.clear();
	listItem.clear();
	listStair.clear();
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
					player->IncreaseNumHeart(5);
					listItem.at(i)->visible = false;
				}
				else if (dynamic_cast<ChainItem*>(listItem.at(i)))
				{
					player->isEatingItem = true;
					listItem.at(i)->visible = false;
					player->whip->LevelUp();
				}
				else if (dynamic_cast<DaggerItem*>(listItem.at(i)))
				{
					player->SetSubWeapon(ID_DAGGER);
					listItem.at(i)->visible = false;
				}
				else if (dynamic_cast<BoomerangItem*>(listItem.at(i)))
				{
					player->SetSubWeapon(ID_BOOMERANG);
					listItem.at(i)->visible = false;
				}
				else if (dynamic_cast<CrownItem*>(listItem.at(i)))
				{
					player->SetScore(2000);
					listItem.at(i)->visible = false;
					listEffet1.at(0)->SetState(EFFECT_STATE_2000);
					listEffet1.at(0)->SetPosition(listItem.at(i)->x, listItem.at(i)->y);
					listEffet1.at(0)->SetVisible(true);
				}
				else if (dynamic_cast<SmallHeartItem*>(listItem.at(i)))
				{
					player->IncreaseNumHeart(1);
					player->SetScore(100);
					listItem.at(i)->visible = false;
				}
				else if (dynamic_cast<MoneyBagItem*>(listItem.at(i)))
				{
					
					if (listItem.at(i)->GetState() == 0)
					{
						listItem.at(i)->visible = false;
						player->SetScore(100);
						listEffet1.at(0)->SetState(EFFECT_STATE_100);
						listEffet1.at(0)->SetPosition(listItem.at(i)->x + 20, listItem.at(i)->y);
						listEffet1.at(0)->SetVisible(true);
					}
					else if (listItem.at(i)->GetState() == 1)
					{
						listItem.at(i)->visible = false;
						player->SetScore(400);
						listEffet1.at(0)->SetState(EFFECT_STATE_400);
						listEffet1.at(0)->SetPosition(listItem.at(i)->x + 20, listItem.at(i)->y);
						listEffet1.at(0)->SetVisible(true);
					}
					else if (listItem.at(i)->GetState() == 2)
					{
						listItem.at(i)->visible = false;
						player->SetScore(700);
						listEffet1.at(0)->SetState(EFFECT_STATE_700);
						listEffet1.at(0)->SetPosition(listItem.at(i)->x + 20, listItem.at(i)->y);
						listEffet1.at(0)->SetVisible(true);
					}
				}
				else if (dynamic_cast<HolyWaterItem*>(listItem.at(i)))
				{
					player->SetSubWeapon(ID_HOLYWATER);
					listItem.at(i)->visible = false;
				}
				else if (dynamic_cast<AxeItem*>(listItem.at(i)))
				{
					player->SetSubWeapon(ID_AXE);
					listItem.at(i)->visible = false;
				}
				else if (dynamic_cast<WatchItem*>(listItem.at(i)))
				{
					player->SetSubWeapon(ID_WATCH);
					listItem.at(i)->visible = false;
				}
			}
		}
	}
}
void CPlayScene::CheckCollision_WhipAndTorch()
{
	player->whip->SetDamage();
	if (player->state == SIMON_STATE_ATTACK 
		|| player->state == SIMON_STATE_SIT_AND_ATTACK 
		|| player->state == SIMON_STATE_ATTACK_UPSTAIR 
		|| player->state == SIMON_STATE_ATTACK_DOWNSTAIR)
	{
		if (	player->animation_set->at(SIMON_ANI_ATTACK)->GetCurrentFrame() == 2 
			|| player->animation_set->at(SIMON_ANI_SIT_AND_ATTACK)->GetCurrentFrame() == 2
			|| player->animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->GetCurrentFrame() == 2 
			|| player->animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->GetCurrentFrame()==2)
		{
			DebugOut(L"Last Frame\n");
			for (UINT i = 0; i < listTorch.size(); i++)
			{
				if (player->whip->AABB(player->whip->GetBound(), listTorch.at(i)->GetBound()))
				{
					listTorch.at(i)->SetState(TORCH_DESTROYED);
					listTorch.at(i)->animation_set->at(TORCH_DESTROYED)->SetAniStartTime(GetTickCount());
				}
			}
		}
	}
}
void CPlayScene::CheckCollision_WhipAndEnemy()
{
	player->whip->SetDamage();
	if (player->GetState() == SIMON_STATE_ATTACK || player->GetState() == SIMON_STATE_SIT_AND_ATTACK || player->GetState() == SIMON_STATE_ATTACK_UPSTAIR || player->GetState()  == SIMON_STATE_ATTACK_DOWNSTAIR)
	{
		if (player->animation_set->at(SIMON_ANI_ATTACK)->GetCurrentFrame() == 2 || player->animation_set->at(SIMON_ANI_SIT_AND_ATTACK)->GetCurrentFrame() == 2
			|| player->animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->GetCurrentFrame() == 2 || player->animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->GetCurrentFrame() == 2)
		{
			{
				for (UINT j = 0; j < listEnemy.size(); j++)
				{
					if (listEnemy.at(j)->isVisible() == true)
					{
						LPGAMEOBJECT temp = listEnemy.at(j);
						if (player->whip->AABB(player->whip->GetBound(), temp->GetBound()))
						{
							if (dynamic_cast<BreakableBrick*>(temp))
							{
								DebugOut(L"[INFO]Whip Collision with Breakable \n");
								if (temp->GetState() == BREAKABLE_BRICK_STATE_ORIGIN)
									temp->SetState(BREAKABLE_BRICK_STATE_HALF_PART);
								else if (temp->GetState() == BREAKABLE_BRICK_STATE_HALF_PART)
								{
									temp->Die();
								}
								Rocks::GetInstance()->DropRock(temp->x, temp->y);
								return;
							}
							else if (dynamic_cast<Knight*>(temp))
							{
								temp->TakeDamage(player->whip->damage);
							}
							else if (dynamic_cast<Bat*>(temp))
							{
								temp->TakeDamage(player->whip->damage);
							}
							else if (dynamic_cast<Ghoul*>(temp))
							{
								temp->TakeDamage(player->whip->damage);
							}
							else if (dynamic_cast<Ghost*>(temp))
							{
								temp->TakeDamage(player->whip->damage);
							}
							else if (dynamic_cast<Skeleton*>(temp))
							{
								temp->TakeDamage(player->whip->damage);
							}
							else if (dynamic_cast<Fleaman*>(temp))
							{
								temp->TakeDamage(player->whip->damage);
							}
							if (listEnemy.at(j)->healthPoint > 0)
							{
								listEffet1.at(0)->SetState(EFFECT_STATE_FLAME);
								listEffet1.at(0)->SetPosition(temp->x, temp->y);
								listEffet1.at(0)->SetVisible(true);
							}
							else
							{
								listEffet1.at(0)->SetState(EFFECT_STATE_DESTROY);
								listEffet1.at(0)->SetPosition(temp->x, temp->y);
								listEffet1.at(0)->SetVisible(true);
							}
						}
					}
				}
			}
		}
	}
}
void CPlayScene::CheckCollision_PortalAndSimon()
{
	for (UINT i = 0; i < listPortal.size(); i++)
	{
		if (player->CheckCollision(listPortal.at(i)))
		{
			CGame::GetInstance()->SwitchScene(this->scene_id);
		}
	}
}
void CPlayScene::CheckCollision_WeaponAndTorch()
{
	for (UINT i = 0; i < listWeapon.size(); i++)
	{
		for (UINT j = 0; j < listTorch.size(); j++)
		if (listWeapon.at(i)->AABB(listTorch.at(j)->GetBound(), listWeapon.at(i)->GetBound()))
		{
			if (dynamic_cast<Dagger*>(listWeapon.at(i)))
			listWeapon.at(i)->SetVisible(false);
			listTorch.at(j)->SetState(TORCH_DESTROYED);
			listTorch.at(j)->animation_set->at(TORCH_DESTROYED)->SetAniStartTime(GetTickCount());
		}
	}
}
void CPlayScene::CheckCollision_SimonAndEnemy()
{
	if (player->GetState() == SIMON_STATE_DIE)
		return;
	if (player->GetState() == SIMON_STATE_DEFLECT)
		return;
	if (player->untouchable)  return;
	for (UINT i = 0; i < listEnemy.size(); i++)
	{
		if (listEnemy.at(i)->isVisible() == true)
		{
			if (player->AABB(listEnemy.at(i)->GetBound(), player->GetBound()))
			{
				if (player->isOnStair)
				{
					player->StartUntouchable();
					return;
				}
				if (dynamic_cast<BreakableBrick*>(listEnemy.at(i)))
				{
					break;
				}
				else
				{
					player->DecreaseHealth();
					player->SetState(SIMON_STATE_DEFLECT);
					if (dynamic_cast<Bat*>(listEnemy.at(i)) || dynamic_cast<Raven*>(listEnemy.at(i)))
					{
						listEnemy.at(i)->healthPoint = 0;
						listEnemy.at(i)->SetVisible(false);
						player->SetScore(100);
					}
				}
			}
		}
	}
}
void CPlayScene::CheckCollision_WeaponAndEnemy()
{
	if (player->GetState() == SIMON_STATE_DEFLECT)
		return;
	for (UINT i = 0; i < listWeapon.size(); i++)
	{
		if(listWeapon.at(i)->isVisible() == true)
		{ 
		for (UINT j = 0; j < listEnemy.size(); j++)
		{
			if (listEnemy.at(j)->isVisible() == true)
			{
				if (dynamic_cast<BreakableBrick*>(listEnemy.at(j)))
				{
					continue;
				}
				if (dynamic_cast<Dagger*>(listWeapon.at(i)))
				{
					if (listWeapon.at(i)->isVisible() == true)
					{
						if (listWeapon.at(i)->AABB(listEnemy.at(j)->GetBound(), listWeapon.at(i)->GetBound()))
						{
							listWeapon.at(i)->SetVisible(false);
							listEnemy.at(j)->TakeDamage(listWeapon.at(i)->damage);
							listEffet1.at(0)->SetState(EFFECT_STATE_FLAME);
							listEffet1.at(0)->SetPosition(listEnemy.at(j)->x, listEnemy.at(j)->y);
							listEffet1.at(0)->SetVisible(true);
						}
					}
				}
				else if (listWeapon.at(i)->AABB(listEnemy.at(j)->GetBound(), listWeapon.at(i)->GetBound()))
				{
					listEnemy.at(j)->TakeDamage(listWeapon.at(i)->damage);
					listEffet1.at(0)->SetState(EFFECT_STATE_FLAME);
					listEffet1.at(0)->SetPosition(listEnemy.at(j)->x, listEnemy.at(j)->y);
					listEffet1.at(0)->SetVisible(true);
				}
			}
		}
		}
	}
}
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon *simon = ((CPlayScene*)scence)->GetPlayer();
	Dagger* dagger = ((CPlayScene*)scence)->GetDagger();
	Boomerang* boomerang = ((CPlayScene*)scence)->GetBoomerang();
	HolyWater* holywater = ((CPlayScene*)scence)->GetHolyWater();
	Axe* axe = ((CPlayScene*)scence)->GetAxe();
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (simon->GetState() == SIMON_STATE_DEFLECT) return;
	if (simon->isEatingItem)
	{
		simon->SetState(SIMON_STATE_IDLE);
		return;
	}
	switch (KeyCode)
	{
	case DIK_A:
	{
		if ((simon->GetState() == SIMON_STATE_ATTACK) || (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK))
			return;
		if ((simon->GetState() == SIMON_STATE_ATTACK_UPSTAIR) || (simon->GetState() == SIMON_STATE_ATTACK_DOWNSTAIR))
			return;
		if (simon->isOnStair)
		{
			if (simon->isUpstair)
			{
				simon->SetState(SIMON_STATE_ATTACK_UPSTAIR);
			}
			else
			{
				simon->SetState(SIMON_STATE_ATTACK_DOWNSTAIR);
			}
		}
		else
		{
			if (simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_JUMP)
			{
				simon->isAttack = true;
				simon->SetState(SIMON_STATE_ATTACK);
			}

			else if (simon->GetState() == SIMON_STATE_SIT) {
				simon->isAttack = true;
				simon->SetState(SIMON_STATE_SIT_AND_ATTACK);
			}
		}
		break;
	}
	case DIK_X:
	{
		if (simon->GetNumHeart() > 0)
		{
			float x, y;
			simon->GetPosition(x, y);
			switch (simon->GetSubWeapon())
			{
			case ID_BOOMERANG:
			{
				if (simon->GetState() == SIMON_STATE_THROW) return;
				if (boomerang->visible) return;
				simon->SetState(SIMON_STATE_THROW);
				boomerang->SetPosition(x, y + 5);
				boomerang->SetOrientation(simon->nx);
				boomerang->SetVisible(true);
				simon->SetNumHeart(simon->GetNumHeart() - 1);
				break;
			}
			case ID_DAGGER:
			{
				if (simon->GetState() == SIMON_STATE_THROW) return;
				if (dagger->visible) return;
				simon->SetState(SIMON_STATE_THROW);
				dagger->SetPosition(x, y + 5);
				dagger->SetOrientation(simon->nx);
				dagger->SetVisible(true);
				simon->SetNumHeart(simon->GetNumHeart() - 1);
				break;
			}
			case ID_HOLYWATER:
			{
				if (simon->GetState() == SIMON_STATE_THROW) return;
				if (holywater->visible) return;
				simon->SetState(SIMON_STATE_THROW);
				holywater->firstx = x;
				holywater->SetPosition(x, y + 5);
				holywater->SetOrientation(simon->nx);
				holywater->SetVisible(true);
				simon->SetNumHeart(simon->GetNumHeart() - 1);
				break;
			}
			case ID_AXE:
			{
				if (simon->GetState() == SIMON_STATE_THROW) return;
				if (axe->visible) return;
				simon->SetState(SIMON_STATE_THROW);
				axe->firstx = x;
				axe->SetPosition(x, y + 5);
				axe->SetOrientation(simon->nx);
				axe->SetVisible(true);
				simon->SetNumHeart(simon->GetNumHeart() - 1);
				break;
			}
			case ID_WATCH:
			{
				simon->isUseStop = true;
				//simon->SetNumHeart(simon->GetNumHeart() - 1);
				break;
			}
			}
		}
		else
		{
			DebugOut(L"NOT OK \n");
		}
		break;
	}
	//phím tắt đổi vũ khí
	case DIK_Q:
	{
		switch (simon->GetSubWeapon())
		{
		case NULL:
			simon->SetSubWeapon(ID_BOOMERANG);
			break;
		case ID_BOOMERANG:
			simon->SetSubWeapon(ID_DAGGER);
			break;
		case ID_DAGGER:
			simon->SetSubWeapon(ID_HOLYWATER);
			break;
		case ID_HOLYWATER:
			simon->SetSubWeapon(ID_AXE);
			break;
		case ID_AXE:
			simon->SetSubWeapon(ID_WATCH);
			break;
		case ID_WATCH:
			simon->SetSubWeapon(ID_BOOMERANG);
			break;
		}
		break;
	}
	case DIK_S:
	{
		if (simon->isAttack) return;
		if (simon->isOnStair) return;
		if (simon->isJumping == false)
		{
			simon->SetState(SIMON_STATE_JUMP);
			simon->isJumping = true;
			if (simon->isOnMoving)
			{
				if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT) || CGame::GetInstance()->IsKeyDown(DIK_LEFT))
				{
					return;
				}
				else
				{
					simon->vx = 0;
				}
			}
		}
		break;
	}
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	Simon* simon = ((CPlayScene*)scence)->GetPlayer();
	vector<LPGAMEOBJECT> listStair = ((CPlayScene*)scence)->GetListStair();
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (simon->GetState() == SIMON_STATE_DEFLECT) return;
	if (simon->isLand) return;
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
	if (simon->GetState() == SIMON_STATE_ATTACK_UPSTAIR && simon->animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->IsOver(SIMON_ATTACK_TIME) == false)
		return;
	if (simon->GetState() == SIMON_STATE_ATTACK_DOWNSTAIR && simon->animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->IsOver(SIMON_ATTACK_TIME) == false)
		return;
	if (simon->GetState() == SIMON_STATE_THROW &&
		simon->animation_set->at(SIMON_ANI_THROW)->IsOver(SIMON_ATTACK_TIME) == false)
		return;
	if (simon->GetState() == SIMON_STATE_DEFLECT &&
		simon->animation_set->at(SIMON_ANI_DEFLECT)->IsOver(SIMON_DEFLECT_TIME) == false)
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
				simon->nx = 1;
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
				simon->nx = -1;
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
				simon->isUpstair = false;
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
				//DebugOut(L"DIK_RIGHT OK \n");
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
					if(simon->nx > 0 && listStair.at(i)->StairTag == CGameObject::StairTypes::ToLeft)
					simon->isHitTopStair = true;
					if (simon->nx < 0 && listStair.at(i)->StairTag == CGameObject::StairTypes::ToRight)
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
	else
	{
		if (simon->isOnStair)
		{
			simon->SetState(SIMON_STATE_ONSTAIR);
		}
		else
			simon->SetState(SIMON_STATE_IDLE);
	}
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
			if ((simon->state == SIMON_STATE_ATTACK) || (simon->state == SIMON_STATE_SIT) || simon->state == SIMON_STATE_ATTACK || simon->isJumping == true)
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
			if ((simon->isAttack) || (simon->state == SIMON_STATE_SIT) || simon->isJumping == true)
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
		simon->isStopOnStair = true;
		simon->vx = 0;
		simon->vy = 0;
		break;
	}
	}
}
