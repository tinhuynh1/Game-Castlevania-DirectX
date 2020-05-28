#pragma once
#include <string>
#include <d3dx9.h>
#include "Utils.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define TILE_HEIGHT			16
#define TILE_WIDTH			16
#define	ID_TILE_MAP	100
using namespace std;
class Map
{
private:
	int left, top, right, bottom, x, y;
public:	 
	Map(int x, int y, int left, int top, int right, int bottom) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
		this->x = x;
		this->y = y;
	};
	void Render();
};
typedef Map* LPMAP;
