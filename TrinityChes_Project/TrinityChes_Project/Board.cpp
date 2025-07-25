#include "Board.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"

//�R���X�g���N�^
CBoard::CBoard()
{
	//���_���̒�`
	float hs = MAX_BOARD_SIZE * SQUARE_SIZE / 2.0f;

	Vertex vtx[] = {
		{{-hs,-hs,0,},{0,0}},
		{{-hs,hs,0,},{0,1}},
		{{hs,-hs,0,},{1,0}},
		{{hs,hs,0,},{1,1}},
	};

	//���_�o�b�t�@�̒�`
	m_pVtx = CreateVertexBuffer(vtx, 4);

	//�e�N�X�`���̓ǂݍ���
	HRESULT hr = LoadTextureFromFile(GetDevice(), "Texture/TrinityChesBoard.png", &m_TexBoard);
	if (FAILED(hr)) { MessageBox(NULL, "Board", "Errorrrrr", MB_OK); };

	

}

//�f�X�g���N�^
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

//�`�揈��
void CBoard::Draw()
{
	//�Ֆʂ̕`��
	SetSpritePos(0.0f, 0.0f);
	SetSpriteScale(1.0f, 1.0f);
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpriteTexture(m_TexBoard);
	DrawSprite(m_pVtx);

	//�ړ��\�ȃ}�X�̕`��
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

//�Ֆʂ̍X�V����
void CBoard::Update()
{

}
