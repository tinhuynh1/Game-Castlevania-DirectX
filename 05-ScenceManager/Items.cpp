#include "Items.h"
Items* Items::__instance = NULL;
void Items::AddItem(int itemType, LPGAMEOBJECT item)
{
	items[itemType].push_back(item);
}
void Items::CheckAndDrop(LPGAMEOBJECT object)
{
	int itemType = object->GetItemId();
	float x, y;
	object->GetPosition(x, y);
	Drop(itemType, x, y);
}
void Items::Drop(int itemType,float x, float y)
{
	for (auto i = items[itemType].begin(); i != items[itemType].end(); ++i)
	{
		if ((*i)->isVisible() == false) {
			(*i)->SetPosition(x, y);
			(*i)->SetVisible(true);
			break;
		}
	}
}
Items* Items::GetInstance()
{
	if (__instance == NULL)
		__instance = new Items();
	return __instance;
}