#include "SceneManager.h"
#include "Scene.h"
#include "TitleScene.h"

CSceneManager::CSceneManager()
	: m_pCurrentScene(nullptr)
	, m_pNextScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::Init()
{
	// 最初はタイトルシーンを入れる
	ChangeScene(new CTitleScene());
}

void CSceneManager::Uninit()
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->Uninit();
		delete m_pCurrentScene;
		m_pCurrentScene = nullptr;
	}
	if (m_pNextScene)
	{
		delete m_pNextScene;
		m_pNextScene = nullptr;
	}
}

void CSceneManager::Update()
{
	// 次のシーンへの切り替え予約が入っていたら切り替える
	if (m_pNextScene)
	{
		if (m_pCurrentScene)
		{
			m_pCurrentScene->Uninit();
			delete m_pCurrentScene;
		}

		m_pCurrentScene = m_pNextScene;
		m_pCurrentScene->Init();
		m_pNextScene = nullptr;
	}

	if (m_pCurrentScene)
	{
		m_pCurrentScene->Update();
	}
}

void CSceneManager::Draw()
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->Draw();
	}
}

void CSceneManager::ChangeScene(CScene* pNextScene)
{
	m_pNextScene = pNextScene;
}