#pragma once
#include "Square.h"
#include "SpriteDrawer.h"
#include "Define.h"
#include "Piece.h"
#include "GameManeger.h"
#include <vector>


class CBoard
{
public:
	CBoard(std::vector<MOVEMENT_CANDIDATE>*);
	~CBoard();
	void Draw();
	void Update();

	void FindMovableArea();
	

private:
	void MakeMovable(CPiece*);

	CSquare  aSquare[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	ID3D11Buffer* m_pVtx;	//頂点バッファ
	ID3D11ShaderResourceView* m_TexBoard;	//テクスチャ
	
	CPiece* m_pPieces[PIECE_NUM];	//６体分の駒

	std::vector<MOVEMENT_CANDIDATE>* m_pvecCandidates;

	MOVEMENT_CANDIDATE m_TmpCandidate;

	int aDirection[8][2] = {
		{-1,-1},
		{ 0,-1},
		{ 1,-1},
		{-1, 0},
		{1,  0},
		{-1, 1},
		{ 0, 1},
		{ 1, 1},
	};
};

