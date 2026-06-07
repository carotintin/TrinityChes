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

class CGameManager
{
public:
	

	static  CGameManager& GetInstance()
	{
		//インスタンスを一個しか作らない
		static  CGameManager instance;

		return instance;
	}

	void Init();
	void Uninit();
	void Draw();
	void Update();
	PLAYER_ID GetTurn();
	void SetTurn(PLAYER_ID);

private:
	CGameManager();
	~CGameManager();

	void ExploleArea();

	CGameManager(const CGameManager&) = delete;	//コピー禁止
	CGameManager& operator = (const CGameManager&) = delete;	//代入禁止

	CBoard*  m_Board;	

	ID3D11Buffer* m_pVtx;	//頂点バッファ
	ID3D11ShaderResourceView* m_TexCheckMate;	//テクスチャ
	ID3D11ShaderResourceView* m_TexJibakuMate;	//テクスチャ

	// アニメーションターンロゴ
	ID3D11ShaderResourceView* m_TexPlayer1Turn = nullptr; // P1用テクスチャ
	ID3D11ShaderResourceView* m_TexPlayer2Turn = nullptr; // P2用テクスチャ
	ID3D11Buffer* m_pVtxLogo = nullptr; // ロゴ描画用の頂点バッファ
	float m_LogoUvOffset = 0.0f; // UVスクロール用の変数
	
	std::vector<MOVEMENT_CANDIDATE> m_vecCandidates;

	PLAYER_ID m_Turn;
};



