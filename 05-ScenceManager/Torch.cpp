#include "Torch.h"
 
Torch::Torch()
{

}
void Torch::Render()
{
	
	animation_set->at(state)->Render(x, y, nx);
	//RenderBoundingBox();
}
void Torch::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + TORCH_BBOX_WIDTH;
	bottom = y + TORCH_BBOX_HEIGHT;
}
void Torch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == TORCH_DESTROYED && animation_set->at(state)->IsOver(TORCH_DESTROYED_EFFECT_TIME))
	{
		this->visible = false;
		Items::GetInstance()->CheckAndDrop(this);
		return;
	}
}