#include "SpriteDrawer.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"
#include "Game.h"
#include "Board.h"
#include "GameManeger.h"


CGameManeger* g_pGameManeger;


//初期化処理
bool InitGame()
{
	g_pGameManeger = new CGameManeger();

	return true;
}

//終了処理
void UninitGame()
{
	if (g_pGameManeger)
	{
		delete g_pGameManeger;
		g_pGameManeger = nullptr;
	}

}

//更新処理
void UpdateGame()
{
	
}

//描画処理
void DrawGame()
{
	g_pGameManeger->Draw();

}

