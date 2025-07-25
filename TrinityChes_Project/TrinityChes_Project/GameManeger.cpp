#include "GameManeger.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"

//�R���X�g���N�^
CGameManeger::CGameManeger()
{
	m_Board = new CBoard();

	m_Pieces[0] = new CKing(PLAYER1);
	m_Pieces[1] = new CBishop(PLAYER1);
	m_Pieces[2] = new CRook(PLAYER1);
	m_Pieces[3] = new CKing(PLAYER2);
	m_Pieces[4] = new CBishop(PLAYER2);
	m_Pieces[5] = new CRook(PLAYER2);


	for (int i = 0; i < PIECE_NUM; ++i)
	{
		m_Pieces[i]->Move(i+1,i+1);
	}
}

//�f�X�g���N�^
CGameManeger::~CGameManeger()
{

}

//�`�揈��
void CGameManeger::Draw()
{
	m_Board->Draw();

	for (int i = 0; i < PIECE_NUM; ++i)
	{
		m_Pieces[i]->Draw();
	}
}

//�X�V����
void CGameManeger::Update()
{

}

void CGameManeger::ExploleArea()
{

}
