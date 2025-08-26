#pragma once
#include <windows.h>

#define REPEAT_THRESHOLD (30)
#define MOVEMENT_THRESHOLD (5)

class CKeyBoard
{
public:
	static CKeyBoard& GetInstance()
	{
		//インスタンスを一個しか作らない
		static CKeyBoard instance;

		return instance;
	}


	
	void KeyboardUpdate();
	bool IsKeyPress(int nVirtKey);
	bool IsKeyTrigger(int nVirtKey);
	bool IsKeyRelease(int nVirtKey);
	bool IsRepeat(int nVirtKey);

private:
	CKeyBoard();

	CKeyBoard(const CKeyBoard&) = delete;	//コピー禁止
	CKeyBoard& operator = (const CKeyBoard&) = delete;	//代入禁止

	BYTE m_keyTable[256];
	BYTE m_oldKeyTable[256];
};