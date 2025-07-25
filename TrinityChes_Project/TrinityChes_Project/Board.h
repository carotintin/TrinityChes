#pragma once
#include "Square.h"
#include "SpriteDrawer.h"
#include "Define.h"




class CBoard
{
public:
	CBoard();
	~CBoard();
	void Draw();
	void Update();

	

private:
	CSquare  aSquare[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	ID3D11Buffer* m_pVtx;	//���_�o�b�t�@
	ID3D11ShaderResourceView* m_TexBoard;	//�e�N�X�`��
	
};

