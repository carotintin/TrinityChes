#pragma once
#include <string>
#include <Windows.h>
#include <map>

class CSoundManager
{
public:
	static CSoundManager& GetInstance()
	{
		static CSoundManager instance;
		return instance;
	}

	void Init();
	void Uninit();

	// BGMの再生・停止 (ループ再生)
	void PlayBGM(const std::string& key);
	void StopBGM(const std::string& key);

	// SEの再生・停止
	void PlaySE(const std::string& key);
	void StopSE(const std::string& key); // ←追加

private:
	CSoundManager();
	~CSoundManager();
	CSoundManager(const CSoundManager&) = delete;
	CSoundManager& operator=(const CSoundManager&) = delete;

	// ファイルを読み込んで登録するための関数
	void Load(const std::string& key, const std::string& filepath);

	// --- 追加：複数同時再生用の管理変数 ---
	std::map<std::string, int> m_SeIndices;
	static const int MAX_CHANNELS = 5; // 1つのSEにつき同時に鳴らせる数
};