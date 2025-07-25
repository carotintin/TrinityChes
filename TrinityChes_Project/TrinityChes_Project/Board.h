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
	ID3D11Buffer* m_pVtx;	//頂点バッファ
	ID3D11ShaderResourceView* m_TexBoard;	//テクスチャ
	
};

