#include "CrownItem.h"


CrownItem::CrownItem()
{
	visible = false;
}
void CrownItem::Render()
{
	float sx, Sy;
	Simon::GetInstance()->GetPosition(sx, Sy);
	
		if (sx > 220)
		{
			isOn = true;
		}
		if (isOn == true)
		{
			animation_set->at(0)->Render(x, y, nx);
		}
	
	//RenderBoundingBox();
}
void CrownItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}
void CrownItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CROWN_BBOX_WIDTH;
	bottom = y +CROWN_BBOX_HEIGHT;
}