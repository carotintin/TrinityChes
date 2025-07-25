#pragma once
#include <windows.h>

#define REPEAT_THRESHOLD (30)
#define MOVEMENT_THRESHOLD (5)

class CKeyBoard
{
public:
	CKeyBoard(void);
	void KeyboardUpdate();
	bool IsKeyPress(int nVirtKey);
	bool IsKeyTrigger(int nVirtKey);
	bool IsKeyRelease(int nVirtKey);
	bool IsRepeat(int nVirtKey);

private:
	BYTE m_keyTable[256];
	BYTE m_oldKeyTable[256];
};

