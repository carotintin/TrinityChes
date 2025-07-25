#pragma once

typedef enum
{
	PLAYER1 = 1,
	PLAYER2,
}PLAYER_ID;


class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	void Draw();
	void Update();

	

private:
	PLAYER_ID m_ID;

};

