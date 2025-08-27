#include "Cursor.h"
#include "KeyBoard.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"


CCursor::CCursor()
{
	m_PosX = 0;
	m_PosY = 0;

	bPressed = false;




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


	HRESULT hr2 = LoadTextureFromFile(GetDevice(), "texture/ChesFrame.png", &m_TexCursor);
	if (FAILED(hr2)) { MessageBox(NULL, "Cursor", "Errorrrrr", MB_OK); };
}

CCursor::~CCursor()
{

}

void CCursor::Draw()
{
	SetSpritePos(-350.0f + m_PosX * 100.0f, -350.0f + m_PosY * 100.0f);
	SetSpriteScale(1.0f, 1.0f);
	SetSpriteColor(1.0f, 1.0f, 1.0f, 0.6f);
	SetSpriteTexture(m_TexCursor);
	DrawSprite(m_pVtx);
}

void CCursor::Update()
{
	bPressed = false;

	if (CKeyBoard::GetInstance().IsKeyTrigger(VK_UP))
	{
		m_PosY--;
		if (m_PosY < 0) m_PosY = 7;
	}
	if (CKeyBoard::GetInstance().IsKeyTrigger(VK_DOWN))
	{
		m_PosY++;
		if (m_PosY > 7) m_PosY = 0;
	}

	if (CKeyBoard::GetInstance().IsKeyTrigger(VK_LEFT))
	{
		m_PosX--;
		if (m_PosX < 0) m_PosX = 7;
	}
	if (CKeyBoard::GetInstance().IsKeyTrigger(VK_RIGHT))
	{
		m_PosX++;
		if (m_PosX > 7) m_PosX = 0;
	}

	if (CKeyBoard::GetInstance().IsKeyTrigger('Z'))
	{
		bPressed = true;
	}


}

void CCursor::GetPos(int* x, int* y)
{
	*x = m_PosX;
	*y = m_PosY;
}

bool CCursor::GetbPress()
{
	return bPressed;
}
