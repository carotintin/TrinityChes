#include "Board.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"
#include "Player.h"
#include "GameManager.h"



//コンストラクタ
CBoard::CBoard(std::vector<MOVEMENT_CANDIDATE> *_pvecCandidates)
{
	m_pPieces[0] = new CKing(PLAYER1);
	m_pPieces[1] = new CBishop(PLAYER1);
	m_pPieces[2] = new CRook(PLAYER1);
	m_pPieces[3] = new CKing(PLAYER2);
	m_pPieces[4] = new CBishop(PLAYER2);
	m_pPieces[5] = new CRook(PLAYER2);

	m_Cursor = new CCursor();

	m_pSelectedPiece = nullptr;

	m_SelectPhase = SELECT_PIECE;

	m_pvecCandidates = _pvecCandidates;

	m_pPieces[0]->Move(3, 1);	//駒の初期位置（おためし）
	m_pPieces[1]->Move(5, 1);	//駒の初期位置（おためし）
	m_pPieces[2]->Move(1, 1);	//駒の初期位置（おためし）
	m_pPieces[3]->Move(4, 6);	//駒の初期位置（おためし）
	m_pPieces[4]->Move(2, 6);	//駒の初期位置（おためし）
	m_pPieces[5]->Move(6, 6);	//駒の初期位置（おためし）

	for (int i = 0; i < PIECE_NUM; ++i)
	{
		
		int x;
		int y;
		m_pPieces[i]->GetPos(&x,&y);	//駒の座標を取得
		aSquare[y][x].SetPiece(m_pPieces[i]);	//マスに駒の情報を入れる

	}


	//頂点情報の定義
	float hs = MAX_BOARD_SIZE * SQUARE_SIZE / 2.0f;

	Vertex vtx[] = {
		{{-hs,-hs,0,},{0,0}},
		{{-hs,hs,0,},{0,1}},
		{{hs,-hs,0,},{1,0}},
		{{hs,hs,0,},{1,1}},
	};

	//頂点バッファの定義
	m_pVtx = CreateVertexBuffer(vtx, 4);

	//テクスチャの読み込み
	HRESULT hr = LoadTextureFromFile(GetDevice(), "Texture/TrinityChesBoard_NEO.png", &m_TexBoard);
	if (FAILED(hr)) { MessageBox(NULL, "Board", "Errorrrrr", MB_OK); };

	FindMovableArea();
	//MakeMovable(m_pPieces[4]);
}

//デストラクタ
CBoard::~CBoard()
{
	if (m_Cursor)
	{
		delete m_Cursor;
		m_Cursor = nullptr;
	}

	if (m_TexBoard)
	{
		m_TexBoard->Release();
		m_TexBoard = nullptr;
	}

	if (m_pVtx)
	{
		m_pVtx->Release();
		m_pVtx = nullptr;
	}
}

//描画処理
void CBoard::Draw()
{
	//盤面の描画
	SetSpritePos(0.0f, 0.0f);
	SetSpriteScale(1.0f, 1.0f);
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpriteTexture(m_TexBoard);
	DrawSprite(m_pVtx);

	//移動可能なマスの描画
	for (int y = 0; y < MAX_BOARD_SIZE; ++y)
	{
		for (int x = 0; x < MAX_BOARD_SIZE; ++x)
		{
			aSquare[y][x].Draw(x, y);
		}
	}

	m_Cursor->Draw();

}

//盤面の更新処理
void CBoard::Update()
{
	m_Cursor->Update();

	switch (m_SelectPhase)
	{
	case SELECT_PIECE:
		if (m_Cursor->GetbPress())
		{


			int x;
			int y;
			m_Cursor->GetPos(&x, &y);

			
			m_pSelectedPiece = (aSquare[y][x].GetPiece());
			if (m_pSelectedPiece != nullptr)
			{
				if (m_pSelectedPiece->GetID() == CGameManager::GetInstance().GetTurn())
				{
					MakeMovable(aSquare[y][x].GetPiece());
					m_SelectPhase = SELECT_DESTINATION;
				}
				
			}
		}
		break;

	case SELECT_DESTINATION:
		if (m_Cursor->GetbPress())
		{
			int x;
			int y;
			m_Cursor->GetPos(&x, &y);


			if (aSquare[y][x].GetState() == CSquare::MOVABLE)
			{
				int prevX;
				int prevY;
				m_pSelectedPiece->GetPos(&prevX, &prevY);
				aSquare[prevY][prevX].SetPiece(nullptr);

				m_pSelectedPiece->Move(x,y);

				aSquare[y][x].SetPiece(m_pSelectedPiece);
				ResetState(m_pSelectedPiece);
				m_SelectPhase = SELECT_PIECE;
				FindMovableArea();

				m_bTurnEnd = true;

			}
			else if (aSquare[y][x].GetState() == CSquare::NORMAL)
			{
				ResetState(m_pSelectedPiece);
				m_SelectPhase = SELECT_PIECE;
			}
			
		}

		break;


	default:
		break;
	}

	
	


}

//移動可能範囲を求める
void CBoard::FindMovableArea()
{
	int Movement;
	int CurrentX;
	int CurrentY;
	int NewX;
	int NewY;
	int EnemyCount = 0;	//自分の駒の移動範囲に入っている敵の駒の数
	PLAYER_ID ID;

	//ベクターをリセットする
	int size;
	size = m_pvecCandidates->size();
	for (int i = 0; i < size; ++i)
	{
		m_pvecCandidates->pop_back();
	}

	ID = CGameManager::GetInstance().GetTurn();

	
	//移動候補を求める
	for (int i = 0; i < PIECE_NUM; ++i)	//駒を一個一個見る
	{
		if(m_pPieces[i]->GetUsed() == true) break;


		m_pPieces[i]->GetPos(&CurrentX, &CurrentY);		//今見てる駒の座標の取得

		//移動量の方向ごと探索
		for (int j = 0; j < 8; ++j)
		{
			Movement = m_pPieces[i]->GetMovement(j);	//今見てる移動量の取得
			//移動量の数だけ一マスずつ探索
			for (int k = 0; k < Movement; ++k)
			{
				//移動先のマスの計算
				NewX = CurrentX + aDirection[j][0] * (k + 1);	
				if (NewX < 0 || NewX >= MAX_BOARD_SIZE) break;	//盤面をはみ出たら終了
				NewY = CurrentY + aDirection[j][1] * (k + 1);
				if (NewY < 0 || NewY >= MAX_BOARD_SIZE) break;	//盤面をはみ出たら終了

				//探索している場所に駒があったら
				if (aSquare[NewY][NewX].ExistPiece())
				{
					//探索元の駒が自分の駒だったら
					if (m_pPieces[i]->GetID() == ID)
					{
						//探索している場所の駒が敵の駒だったら
						if (aSquare[NewY][NewX].GetPiece()->GetID() != ID)
						{
							EnemyCount++;
							if (EnemyCount >= 2)
							{
								//ここにチェックメイトのフラグを立てる
								m_bTrinityCheckMate = true;
								return;
							}
							
						}

						//探索している場所の駒が自分の駒だったら探索終了
						if (aSquare[NewY][NewX].GetPiece()->GetID() != ID)
						{
							break;
						}
								
					}
					//探索元の駒が敵の駒だったら
					else if (m_pPieces[i]->GetID() != ID)
					{
						//探索している場所の駒が自分の駒だったら探索終了
						if (aSquare[NewY][NewX].GetPiece()->GetID() == ID)
						{
							//ここに自爆メイトのフラグを立てる
							m_bJibakuMate = true;
							return;
						}
					}

				}
				

				//ベクターに情報を入れる処理
				m_TmpCandidate.piece = m_pPieces[i];
				m_TmpCandidate.x = NewX;
				m_TmpCandidate.y = NewY;
				m_pvecCandidates->push_back(m_TmpCandidate);
			}
		}

	}

}

bool CBoard::TurnEnd()
{
	return  m_bTurnEnd;
}

void CBoard::ResetTurnEnd()
{
	m_bTurnEnd = false;
}

bool CBoard::GetCheckMate()
{
	return m_bTrinityCheckMate;
}

bool CBoard::GetJibakuMate()
{
	return m_bJibakuMate;
}

void CBoard::MakeMovable(CPiece* _pPiece)
{
	int size = m_pvecCandidates->size();

	for (int i = 0; i < size; ++i)	
	{
		if (_pPiece == (*m_pvecCandidates)[i].piece)
		{
			aSquare[(*m_pvecCandidates)[i].y][(*m_pvecCandidates)[i].x].SetState(CSquare::SQUARE_STATE::MOVABLE);
		}
	}
}

void CBoard::ResetState(CPiece* _pPiece)
{
	int size = m_pvecCandidates->size();

	for (int i = 0; i < size; ++i)
	{
		if (_pPiece == (*m_pvecCandidates)[i].piece)
		{
			aSquare[(*m_pvecCandidates)[i].y][(*m_pvecCandidates)[i].x].SetState(CSquare::SQUARE_STATE::NORMAL);
		}
	}
}
