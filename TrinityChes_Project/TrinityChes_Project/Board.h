#pragma once
#include "Square.h"
#include "SpriteDrawer.h"
#include "Define.h"
#include "Piece.h"
#include "GameManager.h"
#include <vector>
#include "Cursor.h"
#include <time.h>


class CBoard
{
public:
	CBoard(std::vector<MOVEMENT_CANDIDATE>*);
	~CBoard();
	void Draw();
	void Update();

	void FindMovableArea(bool bCheckMate = true);

	bool TurnEnd();
	void ResetTurnEnd();
	bool GetCheckMate();
	bool GetJibakuMate();

	

private:
	void MakeMovable(CPiece*);
	void ResetState(CPiece*);
	void UpdateSetupAreas();
	void UpdateSetupPreview(); // プレビュー更新と配置判定を行う関数

	bool m_bCanSet = false;    // 現在カーソルがある場所に配置可能かどうかのフラグ

	CSquare  aSquare[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	ID3D11Buffer* m_pVtx;	//頂点バッファ
	ID3D11ShaderResourceView* m_TexBoard;	//テクスチャ
	
	CPiece* m_pPieces[PIECE_NUM];	//６体分の駒
	CPiece* m_pSelectedPiece;	//エラヴァれたコマ

	CCursor* m_Cursor;

	bool m_bTurnEnd = false;
	bool m_bTrinityCheckMate = false;
	bool m_bJibakuMate = false;

	struct SetupInfo
	{
		int pieceIndex;    // m_pPieces のインデックス
		PLAYER_ID player;  // 誰の配置ターンか
	};

	std::vector<SetupInfo> m_SetupOrder;
	int m_SetupIndex = 0; // 現在何番目の配置か

	typedef enum
	{
		SETUP_PHASE,        // ← 追加：配置フェーズ
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

