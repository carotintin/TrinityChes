#ifndef __SPRITE_DRAWER_H__
#define __SPRITE_DRAWER_H__

#include <d3d11.h>

struct float3 {
	float x;
	float y;
	float z;
};
struct float2 {
	float x;
	float y;
};

// ���_�̏��
struct Vertex {
	float3 pos;
	float2 uv;
};

/* @brief ������
 * @param[in] width ��ʉ���
 * @param[in] height ��ʏc��
 * @param[in] isYup true-��ʏ������+/false-��ʉ�������+
 */
void InitSpriteDrawer(
	ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UINT width, UINT height, bool isYup = false);
void UninitSpriteDrawer();

/*
 * @�`�揈��
 */
void DrawSprite(ID3D11Buffer* pSprite);

//--- �e��X�v���C�g�̐ݒ�֐� ---
void SetSpritePos(float x, float y);
void SetSpriteAngle(float rad);
void SetSpriteScale(float x, float y);
void SetSpriteUVPos(float u, float v);
void SetSpriteUVScale(float u, float v);
void SetSpriteColor(float r, float g, float b, float a);
void SetSpriteTexture(ID3D11ShaderResourceView* pTexture);


#endif // __SPRITE_DRAWER_H__