#include "TitleScene.h"
#include "KeyBoard.h"
#include "SceneManager.h"
#include "GameScene.h"
#include <Windows.h>

CTitleScene::CTitleScene()
	: m_TitleState(TITLE_LOGO), m_FadeState(FADE_IN), m_TitleAlpha(0.0f)
	, m_WaitTimer(0), m_pVtxTitle(nullptr), m_TexCreatorLogo(nullptr)
	, m_TexTitle(nullptr), m_TexRules(nullptr), m_TexControls(nullptr)
	, m_pVtxStartText(nullptr), m_TexStartText(nullptr)
	, m_StartTextAlpha(1.0f), m_StartTextAlphaSpeed(-0.01f) 
{
}

CTitleScene::~CTitleScene()
{
	Uninit();
}

void CTitleScene::Init()
{
	m_TitleState = TITLE_LOGO;
	m_FadeState = FADE_IN;
	m_TitleAlpha = 0.0f;
	m_WaitTimer = 0;

	// 1920x1280 のポリゴン
	float offsetY = 25.0f;

	Vertex vtxTitle[] = {
		{{ -960.0f, -640.0f + offsetY, 0.0f }, { 0.0f, 0.0f }}, // 左上
		{{ -960.0f,  640.0f + offsetY, 0.0f }, { 0.0f, 1.0f }}, // 左下
		{{  960.0f, -640.0f + offsetY, 0.0f }, { 1.0f, 0.0f }}, // 右上
		{{  960.0f,  640.0f + offsetY, 0.0f }, { 1.0f, 1.0f }}, // 右下
	};
	m_pVtxTitle = CreateVertexBuffer(vtxTitle, 4);

	Vertex vtxStart[] = {
		{{ -400.0f, -60.0f, 0.0f }, { 0.0f, 0.0f }},
		{{ -400.0f,  60.0f, 0.0f }, { 0.0f, 1.0f }},
		{{  400.0f, -60.0f, 0.0f }, { 1.0f, 0.0f }},
		{{  400.0f,  60.0f, 0.0f }, { 1.0f, 1.0f }},
	};
	m_pVtxStartText = CreateVertexBuffer(vtxStart, 4);


	LoadTextureFromFile(GetDevice(), "Texture/MadeBy.png", &m_TexCreatorLogo);
	LoadTextureFromFile(GetDevice(), "Texture/TitleLogo.png", &m_TexTitle);
	LoadTextureFromFile(GetDevice(), "Texture/Rule.png", &m_TexRules);
	LoadTextureFromFile(GetDevice(), "Texture/ConText.png", &m_TexControls);
	LoadTextureFromFile(GetDevice(), "Texture/GameStartText.png", &m_TexStartText);

}

void CTitleScene::Uninit()
{
	if (m_pVtxTitle) { m_pVtxTitle->Release();      m_pVtxTitle = nullptr; }
	if (m_TexCreatorLogo) { m_TexCreatorLogo->Release(); m_TexCreatorLogo = nullptr; }
	if (m_TexTitle) { m_TexTitle->Release();       m_TexTitle = nullptr; }
	if (m_TexRules) { m_TexRules->Release();       m_TexRules = nullptr; }
	if (m_TexControls) { m_TexControls->Release();    m_TexControls = nullptr; }
	if (m_pVtxStartText) { m_pVtxStartText->Release(); m_pVtxStartText = nullptr; }
	if (m_TexStartText) { m_TexStartText->Release(); m_TexStartText = nullptr; }
}

void CTitleScene::Update()
{
	// GameManagerが動いていないのでここでキーボードを更新
	CKeyBoard::GetInstance().KeyboardUpdate();

	switch (m_TitleState)
	{
	case TITLE_LOGO: // フェードイン→待機→フェードアウト
		if (m_FadeState == FADE_IN) {
			m_TitleAlpha += 0.015f;
			if (m_TitleAlpha >= 1.0f) {
				m_TitleAlpha = 1.0f;
				m_FadeState = FADE_STAY;
				m_WaitTimer = 60; // 1秒待機
			}
		}
		else if (m_FadeState == FADE_STAY) {
			m_WaitTimer--;
			if (m_WaitTimer <= 0) m_FadeState = FADE_OUT;
		}
		else if (m_FadeState == FADE_OUT) {
			m_TitleAlpha -= 0.015f;
			if (m_TitleAlpha <= 0.0f) {
				m_TitleAlpha = 0.0f;
				m_TitleState = TITLE_MAIN;
				m_FadeState = FADE_IN;
			}
		}

		// Enterで強制スキップ
		if (CKeyBoard::GetInstance().IsKeyTrigger(VK_RETURN)) {
			m_TitleAlpha = 0.0f;
			m_TitleState = TITLE_MAIN;
			m_FadeState = FADE_IN;
		}
		break;

	case TITLE_MAIN: // フェードインのみ
		if (m_FadeState == FADE_IN) 
		{
			m_TitleAlpha += 0.02f;
			if (m_TitleAlpha >= 1.0f) m_TitleAlpha = 1.0f;
		}

		m_StartTextAlpha += m_StartTextAlphaSpeed;
		if (m_StartTextAlpha >= 1.0f) {
			m_StartTextAlpha = 1.0f;
			m_StartTextAlphaSpeed *= -1.0f; // 反転して薄くしていく
		}
		else if (m_StartTextAlpha <= 0.2f) { // 完全に消えず0.2で踏みとどまる
			m_StartTextAlpha = 0.2f;
			m_StartTextAlphaSpeed *= -1.0f; // 反転して濃くしていく
		}

		if (CKeyBoard::GetInstance().IsKeyTrigger(VK_RETURN)) {
			m_TitleState = TITLE_RULES;
		}
		break;

	case TITLE_RULES: // フェードなし
		if (CKeyBoard::GetInstance().IsKeyTrigger(VK_RETURN)) {
			m_TitleState = TITLE_CONTROLS;
		}
		break;

	case TITLE_CONTROLS: // フェードなし
		if (CKeyBoard::GetInstance().IsKeyTrigger(VK_RETURN)) {
			// 操作方法でEnterを押したらゲーム本編シーンへ移行
			CSceneManager::GetInstance().ChangeScene(new CGameScene());
		}
		break;
	}
}

void CTitleScene::Draw()
{
	SetSpritePos(0.0f, 0.0f);
	SetSpriteScale(1.0f, 1.0f);

	if (m_TitleState == TITLE_RULES || m_TitleState == TITLE_CONTROLS) {
		SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f); // ルールと操作は不透明
	}
	else {
		SetSpriteColor(1.0f, 1.0f, 1.0f, m_TitleAlpha);
	}

	switch (m_TitleState)
	{
	case TITLE_LOGO:     SetSpriteTexture(m_TexCreatorLogo); break;
	case TITLE_MAIN:     SetSpriteTexture(m_TexTitle);       break;
	case TITLE_RULES:    SetSpriteTexture(m_TexRules);       break;
	case TITLE_CONTROLS: SetSpriteTexture(m_TexControls);    break;
	}

	DrawSprite(m_pVtxTitle);

	if (m_TitleState == TITLE_MAIN)
	{
		// 画面下部（Y=350付近）に配置
		SetSpritePos(0.0f, 350.0f);
		SetSpriteScale(1.0f, 1.0f);

		// 背景のフェードイン中は、文字も一緒にふんわり表示させる
		float finalAlpha = m_StartTextAlpha;
		if (m_FadeState == FADE_IN) {
			finalAlpha *= m_TitleAlpha;
		}

		SetSpriteColor(1.0f, 1.0f, 1.0f, finalAlpha);
		SetSpriteTexture(m_TexStartText);
		DrawSprite(m_pVtxStartText);
	}

}