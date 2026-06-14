#include "GameScene.h"
#include "GameManager.h" 

CGameScene::CGameScene()
{
}

CGameScene::~CGameScene()
{
	Uninit();
}

void CGameScene::Init()
{
	CGameManager::GetInstance().Init();
}

void CGameScene::Uninit()
{
	CGameManager::GetInstance().Uninit();
}

void CGameScene::Update()
{
	CGameManager::GetInstance().Update();
}

void CGameScene::Draw()
{
	CGameManager::GetInstance().Draw();
}