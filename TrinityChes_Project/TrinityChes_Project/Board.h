#pragma once
#include "Square.h"
#include "SpriteDrawer.h"
#include "Define.h"
#include "Piece.h"
#include "GameManager.h"
#include <vector>
#include "Cursor.h"


class CBoard
{
public:
	CBoard(std::vector<MOVEMENT_CANDIDATE>*);
	~CBoard();
	void Draw();
	void Update();

	void FindMovableArea();

	bool TurnEnd();
	void ResetTurnEnd();
	bool GetCheckMate();
	bool GetJibakuMate();

	

private:
	void MakeMovable(CPiece*);
	void ResetState(CPiece*);

	CSquare  aSquare[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	ID3D11Buffer* m_pVtx;	//頂点バッファ
	ID3D11ShaderResourceView* m_TexBoard;	//テクスチャ
	
	CPiece* m_pPieces[PIECE_NUM];	//６体分の駒
	CPiece* m_pSelectedPiece;	//エラヴァれたコマ

	CCursor* m_Cursor;

	bool m_bTurnEnd = false;
	bool m_bTrinityCheckMate = false;
	bool m_bJibakuMate = false;

	typedef enum
	{
		SELECT_PIECE,
		SELECT_DESTINATION,
	}SELECT_PHASE;

	SELECT_PHASE m_SelectPhase;

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

