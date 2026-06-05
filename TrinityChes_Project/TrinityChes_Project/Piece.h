#pragma once
#include "SpriteDrawer.h"
#include "Define.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"
#include "Player.h"

class CPiece
{
public:

	CPiece();
	virtual ~CPiece();
	virtual void Draw();
	void Update();

	void Move(int, int);

	void GetPos(int*, int*);
	int GetMovement(int);
	bool GetUsable();
	void SetUsable(bool);
	PLAYER_ID GetID();

protected:
	PLAYER_ID m_ID;
	float m_PosX;
	float m_PosY;
	int Movement[MAX_BOARD_SIZE];
	bool m_bUsable;

	typedef enum
	{
		KING,
		ROOK,
		BISHOP,
	}PIECE_TYPE;


	PIECE_TYPE m_Type;	//駒のタイプ
	ID3D11Buffer* m_pVtx;	//頂点バッファ
	ID3D11ShaderResourceView* m_TexPiece;	//テクスチャ


};

