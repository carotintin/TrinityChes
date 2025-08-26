#pragma once
#include "SpriteDrawer.h"


class CCursor
{
public:
	CCursor();
	~CCursor();
	void Draw();
	void Update();
	void GetPos(int*, int*);
	bool GetbPress();

private:
	int m_PosX;
	int m_PosY;

	bool bPressed;
	ID3D11Buffer* m_pVtx;	//頂点バッファ
	ID3D11ShaderResourceView* m_TexCursor;	//テクスチャ
};

