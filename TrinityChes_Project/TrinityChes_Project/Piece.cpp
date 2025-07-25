#include "Piece.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"

//コンストラクタ
CPiece::CPiece()
{
	//頂点情報の定義
	float hs = SQUARE_SIZE / 2.0f;

	Vertex vtx[] = {
		{{-hs,-hs,0,},{0,0}},
		{{-hs,hs,0,},{0,1}},
		{{hs,-hs,0,},{1,0}},
		{{hs,hs,0,},{1,1}},
	};

	//頂点バッファの定義
	m_pVtx = CreateVertexBuffer(vtx, 4);

	 m_PosX = -1;
	 m_PosY = -1;
	 
	
	 
}

//デストラクタ
CPiece::~CPiece()
{

}

//描画処理
void CPiece::Draw()
{

}

//更新処理
void CPiece::Update()
{

}

//移動処理
void CPiece::Move(int x, int y)
{
	m_PosX += x;
	m_PosY += y;
}

void CPiece::GetPos(int* x, int* y)
{
	*x = m_PosX;
	*y = m_PosY;
}
