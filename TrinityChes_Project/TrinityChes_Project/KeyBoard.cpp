#include "KeyBoard.h"

CKeyBoard::CKeyBoard(void)
{
	GetKeyboardState(m_keyTable);
	memcpy(m_oldKeyTable, m_keyTable, sizeof(m_oldKeyTable));
}

void CKeyBoard::KeyboardUpdate()
{
	memcpy(m_oldKeyTable, m_keyTable, sizeof(m_oldKeyTable));
	GetKeyboardState(m_keyTable);
}


bool CKeyBoard::IsKeyPress(int nVirtKey)
{
	return m_keyTable[nVirtKey] & 0x80;
}

bool CKeyBoard::IsKeyTrigger(int nVirtKey)
{
	return (m_keyTable[nVirtKey] ^ m_oldKeyTable[nVirtKey]) & m_keyTable[nVirtKey] & 0x80;
}

bool CKeyBoard::IsKeyRelease(int nVirtKey)
{
	return (m_keyTable[nVirtKey] ^ m_oldKeyTable[nVirtKey]) & m_oldKeyTable[nVirtKey] & 0x80;
}

bool CKeyBoard::IsRepeat(int nVirtKey)
{
	return true;
}
