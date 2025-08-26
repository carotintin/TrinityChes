#include "Square.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"

//コンストラクタ
CSquare::CSquare()
{

	//頂点情報の定義
	float hs = 50.0f;

	Vertex vtx[] = {
		{{-hs,-hs,0,},{0,0}},
		{{-hs,hs,0,},{0,1}},
		{{hs,-hs,0,},{1,0}},
		{{hs,hs,0,},{1,1}},
	};

	//頂点バッファの定義
	m_pVtx = CreateVertexBuffer(vtx, 4);

	m_State = NOMAL;
	
	m_pPiece = nullptr;

	HRESULT hr2 = LoadTextureFromFile(GetDevice(), "Texture/MovableBlue.png", &m_TexMovable);
	if (FAILED(hr2)) { MessageBox(NULL, "Board", "Errorrrrr", MB_OK); };
}

//デストラクタ
CSquare::~CSquare()
{
	if (m_TexMovable)
	{
		m_TexMovable->Release();
		m_TexMovable = nullptr;
	}

	if (m_pVtx)
	{
		m_pVtx->Release();
		m_pVtx = nullptr;
	}
}

//描画処理
void CSquare::Draw(int x, int y)
{


	if (m_State == MOVABLE)
	{
		SetSpritePos(-350.0f + x * 100.0f, -350.0f + y * 100.0f);
		SetSpriteScale(1.0f, 1.0f);
		SetSpriteColor(1.0f, 1.0f, 1.0f, 0.6f);
		SetSpriteTexture(m_TexMovable);
		DrawSprite(m_pVtx);
	}

	


	if (m_pPiece)
	{
		m_pPiece->Draw();
	}


}

//描画の状態を返す
CSquare::SQUARE_STATE CSquare::GetState()
{
	return m_State;
}

void CSquare::SetState(SQUARE_STATE _state)
{
	m_State = _state;
}

void CSquare::SetPiece(CPiece* _pPiece)
{
	m_pPiece = _pPiece;
}

bool CSquare::ExistPiece()
{
	return m_pPiece;
}



