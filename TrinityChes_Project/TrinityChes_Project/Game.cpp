#include "SpriteDrawer.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"
#include "Game.h"
#include "Board.h"
#include "GameManager.h"


CGameManager* g_pGameManeger;


//初期化処理
bool InitGame()
{
	
	CGameManager::GetInstance().Init();
	return true;
}

//終了処理
void UninitGame()
{
	CGameManager::GetInstance().Uninit();

}

//更新処理
void UpdateGame()
{
	CGameManager::GetInstance().Update();
}

//描画処理
void DrawGame()
{
	CGameManager::GetInstance().Draw();

}

