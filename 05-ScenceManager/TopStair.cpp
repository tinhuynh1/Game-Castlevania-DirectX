#include "TopStair.h"
TopStair::TopStair()
{
	//this->visible = false;
}
void TopStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TOPSTAIR_BBOX_WIDTH;
	bottom = y + TOPSTAIR_BBOX_HEIGHT;
}
void TopStair::Render()
{
	//animation_set->at(0)->Render(x, y, nx, 110);
	RenderBoundingBox();
}