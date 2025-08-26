#pragma once
#include "Piece.h"
#include "Define.h"
#include "Player.h"
#include <vector>


typedef struct {
	CPiece* piece;
	int x;
	int y;
}MOVEMENT_CANDIDATE;

class CBoard;

class CGameManeger
{
public:
	CGameManeger();
	~CGameManeger();

	void Draw();
	void Update();

private:
	void ExploleArea();

	CBoard*  m_Board;	

	
	std::vector<MOVEMENT_CANDIDATE> m_vecCandidates;


};



