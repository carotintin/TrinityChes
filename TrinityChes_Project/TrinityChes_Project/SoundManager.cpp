#include "SoundManager.h"
#include <mmsystem.h> // mciSendString を使うため

CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
}

void CSoundManager::Init()
{
	// ========================================================
	// 起動時にすべての音ファイルを読み込み、キーワード(エイリアス)を付ける
	// ========================================================

	// BGMの読み込み
	Load("GameBGM", "Sound/MyCheapAM25.mp3");

	// SEの読み込み
	Load("LogoSE", "Sound/Scene-Flashback05-1(Short).mp3");
	Load("TitleSE", "Sound/Scene-Flashback01-1(Mid).mp3");
	Load("MoveSE", "Sound/Move.mp3");
	Load("SetSE", "Sound/Set.mp3");
	Load("EnterSE", "Sound/Enter.mp3");
	Load("GameOver", "Sound/GameOver.mp3");
}

void CSoundManager::Uninit()
{
	// 全ての音声を閉じてメモリを解放する
	mciSendString("close all", NULL, 0, NULL);
}

void CSoundManager::Load(const std::string& key, const std::string& filepath)
{
	// 同じ音を重ねて鳴らせるように、末尾に _0, _1 ... と付けて5個ずつロードする
	for (int i = 0; i < MAX_CHANNELS; ++i)
	{
		std::string alias = key + "_" + std::to_string(i);
		std::string command = "open \"" + filepath + "\" alias " + alias;
		mciSendString(command.c_str(), NULL, 0, NULL);
	}
	m_SeIndices[key] = 0; // 再生インデックスを初期化
}

void CSoundManager::PlayBGM(const std::string& key)
{
	// BGMは0番のチャンネルを使ってループ再生
	std::string alias = key + "_0";
	std::string command = "play " + alias + " from 0 repeat";
	mciSendString(command.c_str(), NULL, 0, NULL);
}

void CSoundManager::StopBGM(const std::string& key)
{
	std::string alias = key + "_0";
	std::string command = "stop " + alias;
	mciSendString(command.c_str(), NULL, 0, NULL);
}

void CSoundManager::PlaySE(const std::string& key)
{
	// 現在のインデックスを取得（次に鳴らすのは何番か）
	int index = m_SeIndices[key];
	std::string alias = key + "_" + std::to_string(index);

	// 指定したチャンネルを最初から再生
	std::string command = "play " + alias + " from 0";
	mciSendString(command.c_str(), NULL, 0, NULL);

	// 次に鳴らすときは次のチャンネルを使うようにインデックスを進める (0～4でループ)
	m_SeIndices[key] = (index + 1) % MAX_CHANNELS;
}

void CSoundManager::StopSE(const std::string& key)
{
	// このSEの全チャンネルの音をストップする
	for (int i = 0; i < MAX_CHANNELS; ++i)
	{
		std::string alias = key + "_" + std::to_string(i);
		std::string command = "stop " + alias;
		mciSendString(command.c_str(), NULL, 0, NULL);
	}
}