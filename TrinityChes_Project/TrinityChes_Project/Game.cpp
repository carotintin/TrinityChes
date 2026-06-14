#include "SpriteDrawer.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"
#include "Game.h"
#include "SceneManager.h" 

//初期化処理
bool InitGame()
{
	CSceneManager::GetInstance().Init();
	return true;
}

//終了処理
void UninitGame()
{
	CSceneManager::GetInstance().Uninit();
}

//更新処理
void UpdateGame()
{
	CSceneManager::GetInstance().Update();
}

//描画処理
void DrawGame()
{
	CSceneManager::GetInstance().Draw();
}