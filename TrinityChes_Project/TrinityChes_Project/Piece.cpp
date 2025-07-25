#include "Piece.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"

//�R���X�g���N�^
CPiece::CPiece()
{
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

	 m_PosX = -1;
	 m_PosY = -1;
	 
	
	 
}

//�f�X�g���N�^
CPiece::~CPiece()
{

}

//�`�揈��
void CPiece::Draw()
{

}

//�X�V����
void CPiece::Update()
{

}

//�ړ�����
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
