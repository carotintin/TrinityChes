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

protected:
	PLAYER_ID m_ID;
	float m_PosX;
	float m_PosY;
	int Movement[MAX_BOARD_SIZE];

	ID3D11Buffer* m_pVtx;	//���_�o�b�t�@
	ID3D11ShaderResourceView* m_TexPiece;	//�e�N�X�`��


};

