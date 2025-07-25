#pragma once
#include "SpriteDrawer.h"

class CSquare
{
public:
	CSquare();
	~CSquare();
	void Draw(int, int);

	
	
	typedef enum
	{
		NOMAL,
		MOVABLE,
	}SQUARE_STATE;

	typedef enum
	{
		KING,
		ROOK,
		BISHOP,
	}PIECE_TYPE;

	SQUARE_STATE GetState();


private:

	SQUARE_STATE m_State;
	PIECE_TYPE m_Type;

	ID3D11Buffer* m_pVtx;	//頂点バッファ
	ID3D11ShaderResourceView* m_TexMovable;	//テクスチャ
};

