#pragma once
#include "Scene.h"
#include "SpriteDrawer.h"
#include "DirectX.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"

class CTitleScene : public CScene
{
public:
	CTitleScene();
	virtual ~CTitleScene();

	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	typedef enum {
		TITLE_LOGO,     // 制作者ロゴ画面
		TITLE_MAIN,     // タイトル画面
		TITLE_RULES,    // ルール説明画面
		TITLE_CONTROLS  // 操作方法画面
	} TITLE_STATE;

	typedef enum {
		FADE_IN,
		FADE_STAY,
		FADE_OUT
	} FADE_STATE;

	TITLE_STATE m_TitleState;
	FADE_STATE  m_FadeState;
	float       m_TitleAlpha;
	int         m_WaitTimer;

	float m_StartTextAlpha;      // テキストの透明度
	float m_StartTextAlphaSpeed; // 透明度の変化スピード

	ID3D11Buffer* m_pVtxTitle;
	ID3D11Buffer* m_pVtxStartText;

	ID3D11ShaderResourceView* m_TexCreatorLogo;
	ID3D11ShaderResourceView* m_TexTitle;
	ID3D11ShaderResourceView* m_TexRules;
	ID3D11ShaderResourceView* m_TexControls;
	ID3D11ShaderResourceView* m_TexStartText;
};