#include "Skeleton.h"
#include "Brick.h"
Skeleton::Skeleton(Simon* simon)
{
	mSimon = simon;
	this->healthPoint = 1;
	this->visible = false;
	SetState(SKELETON_STATE_MOVE);
}
Skeleton::~Skeleton()
{
}
void  Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + SKELETON_BBOX_WIDTH;
	bottom = top + SKELETON_BBOX_HEIGHT;
}
void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CGameObject::SetState(state);
	if (abs(mSimon->GetInstance()->GetPosition().x - x) > 32)
	{
		nx = (mSimon->GetInstance()->GetPosition().x > x) ? 1 : -1;
	}

	//khoảng cách skeleton và simon nhỏ hơn 70 tì xuất hiện
	if (abs(mSimon->GetInstance()->GetPosition().x-this->x) < 100 && this->healthPoint>0)
	{
		SetVisible(true);
	}

	if (isVisible() == false)
	{
		return;
	}
	if (start_untouchable != 0)
	{
		Untouchable();
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (isStop)
	{
		vx = 0;
		vy = 0;
	}
	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
		vy += 0.0018f * dt;
		//SetState(SKELETON_STATE_JUMP);
	}
	else
	{
		SetState(SKELETON_STATE_MOVE);
		float min_tx, min_ty, nx, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		x += min_tx * dx;
		y += min_ty * dy;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				// The limmied of the knight is the width of the bricks under its feet
				CBrick* b = dynamic_cast<CBrick*>(e->obj);
				if (e->ny != 0)
				{
					int r = rand() % 30;
					if (r == 0)
					{
						isCreateBone = true;
					}
					if (GetState() == SKELETON_STATE_MOVE)
					{
						if (e->ny == -1)
						{
							vy = 0;
							y += ny * 0.1f;
							{
								if (abs(mSimon->GetInstance()->GetPosition().x - x) < 64)
								{
									vx = (mSimon->GetInstance()->GetPosition().x < x) ? 0.05 : -0.05;
								}
								if (abs(mSimon->GetInstance()->GetPosition().x - x) > 82)
								{
									vx = (mSimon->GetInstance()->GetPosition().x > x) ? 0.05 : -0.05;
								}
							}
						}
					}
					//else if (GetState() == SKELETON_STATE_JUMP)
					//{
					//	y += ny * 0.1f;
					//	/*if (this->y < 96)
					//	{
					//		vy = -0.3f ;
					//		vx = -0.15f;
					//	}
					//	else
					//	{
					//		vy = -0.35;
					//		vx = 0.15;
					//	}*/
					//}
					if (e->ny == -1)
					{
						if (this->x > (b->x + b->GetWidth() - 8))
						{
							//cuối gạch thì nhảy chéo lên phải
							vy = -0.37f;
							vx = 0.14f;
						}
						/*else*/ if (this->x < b->x && this->x > mSimon->GetInstance()->GetPosition().x || abs(this->y - mSimon->GetInstance()->GetPosition().y) > 64)
						{
							//đầu gạch thì nhảy chéo qua trái
							vy = -0.35f;
							vx = -0.15f;
						}	
					}
				}
			}
		}
	}
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Skeleton::Render()
{
	if (!isStop)
		animation_set->at(0)->Render(x, y, nx);
	else
	{
		animation_set->at(0)->RenderByFrame(animation_set->at(0)->GetCurrentFrame(), nx, x, y);
	}
	//RenderBoundingBox();
}
RECT Skeleton::GetBound()
{
	return CGameObject::GetBound();
}