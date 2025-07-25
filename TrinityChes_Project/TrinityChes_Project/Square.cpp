#include "Square.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"

//�R���X�g���N�^
CSquare::CSquare()
{

	//���_���̒�`
	float hs = 50.0f;

	Vertex vtx[] = {
		{{-hs,-hs,0,},{0,0}},
		{{-hs,hs,0,},{0,1}},
		{{hs,-hs,0,},{1,0}},
		{{hs,hs,0,},{1,1}},
	};

	//���_�o�b�t�@�̒�`
	m_pVtx = CreateVertexBuffer(vtx, 4);

	m_State = NOMAL;
	m_Type = KING;

	HRESULT hr2 = LoadTextureFromFile(GetDevice(), "Texture/MovableBlue.png", &m_TexMovable);
	if (FAILED(hr2)) { MessageBox(NULL, "Board", "Errorrrrr", MB_OK); };
}

//�f�X�g���N�^
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

//�`�揈��
void CSquare::Draw(int x, int y)
{
	SetSpritePos(-350.0f + x * 100.0f, -350.0f + y * 100.0f);
	SetSpriteScale(1.0f, 1.0f);
	SetSpriteColor(1.0f, 1.0f, 1.0f, 0.6f);
	SetSpriteTexture(m_TexMovable);
	DrawSprite(m_pVtx);
}

//�`��̏�Ԃ�Ԃ�
CSquare::SQUARE_STATE CSquare::GetState()
{
	return m_State;
}



