#include "GameManager.h"
#include "Board.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"
#include "KeyBoard.h"
#include <Windows.h>
#include "SoundManager.h"


//コンストラクタ
CGameManager::CGameManager()
{
	
}

//デストラクタ
CGameManager::~CGameManager()
{

}

void CGameManager::Init()
{
	m_Board = new CBoard(&m_vecCandidates);

	CSoundManager::GetInstance().PlayBGM("GameBGM");

	Vertex vtx[] = {
		{{ -960.0f, -540.0f, 0.0f }, { 0.0f, 0.0f }}, // 左上（Yマイナス、UV 0,0）
		{{ -960.0f,  540.0f, 0.0f }, { 0.0f, 1.0f }}, // 左下（Yプラス、UV 0,1）
		{{  960.0f, -540.0f, 0.0f }, { 1.0f, 0.0f }}, // 右上（Yマイナス、UV 1,0）
		{{  960.0f,  540.0f, 0.0f }, { 1.0f, 1.0f }}, // 右下（Yプラス、UV 1,1）
	};

	//頂点バッファの定義
	m_pVtx = CreateVertexBuffer(vtx, 4);

	//テクスチャの読み込み
	HRESULT hr = LoadTextureFromFile(GetDevice(), "Texture/CheckMate.png", &m_TexCheckMate);
	if (FAILED(hr)) { MessageBox(NULL, "CheckMate", "Errorrrrr", MB_OK); };
	HRESULT hr2 = LoadTextureFromFile(GetDevice(), "Texture/JibakuMate.png", &m_TexJibakuMate);
	if (FAILED(hr2)) { MessageBox(NULL, "JibakuMate", "Errorrrrr", MB_OK); };


	// テクスチャの読み込み
	LoadTextureFromFile(GetDevice(), "Texture/PLAYER1TURN.png", &m_TexPlayer1Turn);
	LoadTextureFromFile(GetDevice(), "Texture/PLAYER2TURN.png", &m_TexPlayer2Turn);


	// 縦長ポリゴン
	Vertex vtxLogo[] = {
		{{-400, -540, 0}, {0, 0}},
		{{-400,  540, 0}, {0, 1}},
		{{ 400, -540, 0}, {1, 0}},
		{{ 400,  540, 0}, {1, 1}},
	};
	m_pVtxLogo = CreateVertexBuffer(vtxLogo, 4);
	bEndBGM = false;
}

void CGameManager::Uninit()
{
	// 盤面の解放
	if (m_Board)
	{
		delete m_Board;
		m_Board = nullptr;
	}

	// 頂点バッファの解放
	if (m_pVtx)
	{
		m_pVtx->Release();
		m_pVtx = nullptr;
	}
	if (m_pVtxLogo)
	{
		m_pVtxLogo->Release();
		m_pVtxLogo = nullptr;
	}

	// テクスチャの解放
	if (m_TexCheckMate)
	{
		m_TexCheckMate->Release();
		m_TexCheckMate = nullptr;
	}
	if (m_TexJibakuMate)
	{
		m_TexJibakuMate->Release();
		m_TexJibakuMate = nullptr;
	}
	if (m_TexPlayer1Turn)
	{
		m_TexPlayer1Turn->Release();
		m_TexPlayer1Turn = nullptr;
	}
	if (m_TexPlayer2Turn)
	{
		m_TexPlayer2Turn->Release();
		m_TexPlayer2Turn = nullptr;
	}
}

//描画処理
void CGameManager::Draw()
{
	//盤面の描画

	m_Board->Draw();


	// --- プレイヤーターンロゴの描画 ---

	// 現在のターンに合わせてテクスチャを選択
	ID3D11ShaderResourceView* pCurrentTex = nullptr;
	if (m_Turn == PLAYER1)
	{
		pCurrentTex = m_TexPlayer1Turn; 
	}
	else
	{
		pCurrentTex = m_TexPlayer2Turn; 
	}

	// 選択したテクスチャをセット
	SetSpriteTexture(pCurrentTex);

	float UV = m_LogoUvOffset;

	float uScale = 1.0f;

	SetSpriteScale(1.0f, 1.0f);
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 1.右側の描画
	Vertex vtxRight[] = {
		{{-400, -540, 0},{UV + uScale + 0.6f, 0.0f}},
		{{-400,  540, 0},{UV + 0.6f,          0.0f}},
		{{ 400, -540, 0},{UV + uScale + 0.6f, 1.0f}},
		{{ 400,  540, 0},{UV + 0.6f,          1.0f}},
	};
	GetContext()->UpdateSubresource(m_pVtxLogo, 0, NULL, vtxRight, 0, 0);
	// 画面右端に配置
	SetSpritePos(760.0f, 0.0f);
	DrawSprite(m_pVtxLogo);

	// 2.左側の描画
	Vertex vtxLeft[] = {
		{{-400, -540, 0}, {UV + uScale, 0.0f}},
		{{-400,  540, 0}, {UV,          0.0f}},
		{{ 400, -540, 0}, {UV + uScale, 1.0f}},
		{{ 400,  540, 0}, {UV,          1.0f}},
	};
	GetContext()->UpdateSubresource(m_pVtxLogo, 0, NULL, vtxLeft, 0, 0);
	// 画面左端に配置
	SetSpritePos(-660.0f, 0.0f);
	DrawSprite(m_pVtxLogo);


	//盤面の描画
	if (m_Board->GetCheckMate())
	{
		SetSpritePos(0.0f, 0.0f);
		SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);
		SetSpriteTexture(m_TexCheckMate);
		DrawSprite(m_pVtx);
	}

	if (m_Board->GetJibakuMate())
	{
		SetSpritePos(0.0f, 0.0f);
		SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);
		SetSpriteTexture(m_TexJibakuMate);
		DrawSprite(m_pVtx);
	}
	
}

//更新処理
void CGameManager::Update()
{
	CKeyBoard::GetInstance().KeyboardUpdate();

	// スクロール
	m_LogoUvOffset += 0.0025f;
	if (m_LogoUvOffset > 1.0f)
	{
		m_LogoUvOffset -= 1.0f;
	}

	// ==========================================
	// 勝敗が決まっている場合の処理
	// ==========================================
	if (m_Board->GetCheckMate() || m_Board->GetJibakuMate())
	{
		if (!bEndBGM)
		{
			CSoundManager::GetInstance().StopBGM("GameBGM");
			CSoundManager::GetInstance().PlaySE("GameOver");
			bEndBGM = true;
		}

		// Rキーで初めからやり直し
		if (CKeyBoard::GetInstance().IsKeyTrigger('R'))
		{
			Uninit(); // 古い盤面とリソースを解放
			Init();   // 新しく初期化
		}

		// Enterキーでアプリを終了
		if (CKeyBoard::GetInstance().IsKeyTrigger(VK_RETURN))
		{
			PostQuitMessage(0); // ウィンドウに終了メッセージを送る
		}

		// 勝敗が決まっている間は盤面のUpdateなどを実行せずに終了する
		return;
	}
	// ==========================================

	// 勝負が続いている時のみ盤面を更新
	m_Board->Update();

	//ターンが終わったら
	if (m_Board->TurnEnd())
	{
		// チェックメイト（または自爆メイト）ではない場合のみターンを交代する
		if (!m_Board->GetCheckMate() && !m_Board->GetJibakuMate())
		{
			if (m_Turn == PLAYER1) m_Turn = PLAYER2;
			else if (m_Turn == PLAYER2) m_Turn = PLAYER1;
		}
		// ----------------------------------------------------

		m_Board->ResetTurnEnd();
	}
}

PLAYER_ID CGameManager::GetTurn()
{
	return m_Turn;
}

void CGameManager::SetTurn(PLAYER_ID _turn)
{
	m_Turn = _turn;
}

void CGameManager::ExploleArea()
{

}
