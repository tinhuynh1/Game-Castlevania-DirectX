#include "Scence.h"
//CScene* CScene::__instance = NULL;
//
//CScene* CScene::GetInstance()
//{
//
//	if (__instance == NULL) __instance = new CScene(id, sceneFilePath);
//	return __instance;
//}
CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}