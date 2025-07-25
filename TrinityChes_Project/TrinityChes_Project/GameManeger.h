#pragma once
#include "Board.h"
#include "Piece.h"
#include "Define.h"
#include "Player.h"

class CGameManeger
{
public:
	CGameManeger();
	~CGameManeger();

	void Draw();
	void Update();

private:
	CBoard*  m_Board;	

	CPiece* m_Pieces[PIECE_NUM];	//‚U‘Ì•ª‚Ì‹î


};

