#pragma once

class CScene; // ‘O•ûéŒ¾

class CSceneManager
{
public:
	static CSceneManager& GetInstance()
	{
		static CSceneManager instance;
		return instance;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	// Ÿ‚ÌƒV[ƒ“‚ğ—\–ñ‚·‚é
	void ChangeScene(CScene* pNextScene);

private:
	CSceneManager();
	~CSceneManager();
	CSceneManager(const CSceneManager&) = delete;
	CSceneManager& operator=(const CSceneManager&) = delete;

	CScene* m_pCurrentScene;
	CScene* m_pNextScene;
};