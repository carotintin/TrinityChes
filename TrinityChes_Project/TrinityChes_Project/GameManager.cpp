#include "GameManager.h"
#include "Board.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"
#include "KeyBoard.h"

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

	m_Turn = PLAYER1;

	Vertex vtx[] = {
		{{-800,-800,0,},{0,0}},
		{{-800,800,0,},{0,1}},
		{{800,-800,0,},{1,0}},
		{{800,800,0,},{1,1}},
	};

	//頂点バッファの定義
	m_pVtx = CreateVertexBuffer(vtx, 4);

	//テクスチャの読み込み
	HRESULT hr = LoadTextureFromFile(GetDevice(), "Texture/CheckMate.png", &m_TexCheckMate);
	if (FAILED(hr)) { MessageBox(NULL, "CheckMate", "Errorrrrr", MB_OK); };
	HRESULT hr2 = LoadTextureFromFile(GetDevice(), "Texture/JibakuMate.png", &m_TexJibakuMate);
	if (FAILED(hr2)) { MessageBox(NULL, "JibakuMate", "Errorrrrr", MB_OK); };
}

void CGameManager::Uninit()
{

}

//描画処理
void CGameManager::Draw()
{
	m_Board->Draw();

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
	m_Board->Update();
	
	//ターンが終わったら
	if (m_Board->TurnEnd())
	{
		if (m_Turn == PLAYER1)m_Turn = PLAYER2;
		else if (m_Turn == PLAYER2)m_Turn = PLAYER1;

		m_Board->ResetTurnEnd();
	}

}

PLAYER_ID CGameManager::GetTurn()
{
	return m_Turn;
}

void CGameManager::ExploleArea()
{

}
