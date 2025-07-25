#include "Bishop.h"

//�R���X�g���N�^
CBishop::CBishop(PLAYER_ID _ID)
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
		Movement[i] = 0;
	}

	Movement[0] = MAX_BOARD_SIZE;
	Movement[2] = MAX_BOARD_SIZE;
	Movement[5] = MAX_BOARD_SIZE;
	Movement[7] = MAX_BOARD_SIZE;

	
	//ID�ɉ����ăe�N�X�`����ǂݍ���

	HRESULT hr;

	switch (m_ID)
	{
	case PLAYER1:
		hr = LoadTextureFromFile(GetDevice(), "Texture/HolyBishop.PNG", &m_TexPiece);
		if (FAILED(hr)) { MessageBox(NULL, "Bishop", "Errorrrrr", MB_OK); };
		break;

	case PLAYER2:
		hr = LoadTextureFromFile(GetDevice(), "Texture/DarkBishop.PNG", &m_TexPiece);
		if (FAILED(hr)) { MessageBox(NULL, "Bishop", "Errorrrrr", MB_OK); };
		break;
	}

	
}

//�f�X�g���N�^
CBishop::~CBishop()
{

}

//�`�揈��
void CBishop::Draw()
{
	SetSpritePos(-350.0f + m_PosX * 100.0f, -350.0f + m_PosY * 100.0f);
	SetSpriteScale(1.0f, 1.0f);
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpriteTexture(m_TexPiece);
	DrawSprite(m_pVtx);
}

//�X�V����
void CBishop::Update()
{

}
