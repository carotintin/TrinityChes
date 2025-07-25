#include "Board.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"

//コンストラクタ
CBoard::CBoard()
{
	//頂点情報の定義
	float hs = MAX_BOARD_SIZE * SQUARE_SIZE / 2.0f;

	Vertex vtx[] = {
		{{-hs,-hs,0,},{0,0}},
		{{-hs,hs,0,},{0,1}},
		{{hs,-hs,0,},{1,0}},
		{{hs,hs,0,},{1,1}},
	};

	//頂点バッファの定義
	m_pVtx = CreateVertexBuffer(vtx, 4);

	//テクスチャの読み込み
	HRESULT hr = LoadTextureFromFile(GetDevice(), "Texture/TrinityChesBoard.png", &m_TexBoard);
	if (FAILED(hr)) { MessageBox(NULL, "Board", "Errorrrrr", MB_OK); };

	

}

//デストラクタ
CBoard::~CBoard()
{
	if (m_TexBoard)
	{
		m_TexBoard->Release();
		m_TexBoard = nullptr;
	}

	if (m_pVtx)
	{
		m_pVtx->Release();
		m_pVtx = nullptr;
	}
}

//描画処理
void CBoard::Draw()
{
	//盤面の描画
	SetSpritePos(0.0f, 0.0f);
	SetSpriteScale(1.0f, 1.0f);
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpriteTexture(m_TexBoard);
	DrawSprite(m_pVtx);

	//移動可能なマスの描画
	for (int y = 0; y < MAX_BOARD_SIZE; ++y)
	{
		for (int x = 0; x < MAX_BOARD_SIZE; ++x)
		{
			if (aSquare[y][x].GetState() == CSquare::MOVABLE)
			{
				aSquare[y][x].Draw(x, y);
			}
		}
	}

}

//盤面の更新処理
void CBoard::Update()
{

}
