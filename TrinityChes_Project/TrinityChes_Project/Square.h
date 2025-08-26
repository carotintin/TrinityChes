#pragma once
#include "SpriteDrawer.h"
#include "Piece.h"

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

	

	SQUARE_STATE GetState();
	void SetState(SQUARE_STATE);
	void SetPiece(CPiece*);
	bool ExistPiece();

private:

	SQUARE_STATE m_State;
	
	CPiece* m_pPiece;

	ID3D11Buffer* m_pVtx;	//頂点バッファ
	ID3D11ShaderResourceView* m_TexMovable;	//テクスチャ
};

