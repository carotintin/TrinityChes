#include "GameManeger.h"
#include "Board.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"

//コンストラクタ
CGameManeger::CGameManeger()
{
	m_Board = new CBoard(&m_vecCandidates);

	
}

//デストラクタ
CGameManeger::~CGameManeger()
{

}

//描画処理
void CGameManeger::Draw()
{
	m_Board->Draw();

	
}

//更新処理
void CGameManeger::Update()
{

}

void CGameManeger::ExploleArea()
{

}
