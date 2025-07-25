#include "King.h"

//�R���X�g���N�^
CKing::CKing(PLAYER_ID _ID)
{
	//ID�̏�����
	m_ID = _ID;

	//���_���̒�`
	float hs = SQUARE_SIZE / 2.0f;

	Vertex vtx[] = {
		{{-hs,-hs,0,},{0,0}},
		{{-hs,hs,0,},{0,1}},
		{{hs,-hs,0,},{1,0}},
		{{hs,hs,0,},{1,1}},
	};

	//���_�o�b�t�@�̒�`
	m_pVtx = CreateVertexBuffer(vtx, 4);

	for (int i = 0; i < 8; ++i)
	{
		Movement[i] = 1;
	}

	
	//ID�ɉ����ăe�N�X�`����ǂݍ���

	HRESULT hr;

	switch (m_ID)
	{
	case PLAYER1:
		hr = LoadTextureFromFile(GetDevice(), "Texture/HolyKing.PNG", &m_TexPiece);
		if (FAILED(hr)) { MessageBox(NULL, "King", "Errorrrrr", MB_OK); };
		break;

	case PLAYER2:
		hr = LoadTextureFromFile(GetDevice(), "Texture/DarkKing.PNG", &m_TexPiece);
		if (FAILED(hr)) { MessageBox(NULL, "King", "Errorrrrr", MB_OK); };
		break;
	}




	
}

//�f�X�g���N�^
CKing::~CKing()
{

}

//�`�揈��
void CKing::Draw()
{
	SetSpritePos(-350.0f + m_PosX * 100.0f, -350.0f + m_PosY * 100.0f);
	SetSpriteScale(1.0f, 1.0f);
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpriteTexture(m_TexPiece);
	DrawSprite(m_pVtx);
}

//�X�V����
void CKing::Update()
{

}
