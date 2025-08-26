#include "Board.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"



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

	m_pvecCandidates = _pvecCandidates;

	for (int i = 0; i < PIECE_NUM; ++i)
	{
		m_pPieces[i]->Move(i + 1, i + 1);	//駒の初期位置（おためし）

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
	HRESULT hr = LoadTextureFromFile(GetDevice(), "Texture/TrinityChesBoard.png", &m_TexBoard);
	if (FAILED(hr)) { MessageBox(NULL, "Board", "Errorrrrr", MB_OK); };

	FindMovableArea();
	MakeMovable(m_pPieces[4]);
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
}

//移動可能範囲を求める
void CBoard::FindMovableArea()
{
	int Movement;
	int CurrentX;
	int CurrentY;
	int NewX;
	int NewY;

	for (int i = 0; i < PIECE_NUM; ++i)	//駒を一個一個見る
	{
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

				if (aSquare[NewY][NewX].ExistPiece()) break;		//既に駒があったら終了

				//ベクターに情報を入れる処理
				m_TmpCandidate.piece = m_pPieces[i];
				m_TmpCandidate.x = NewX;
				m_TmpCandidate.y = NewY;
				m_pvecCandidates->push_back(m_TmpCandidate);
			}
		}

	}

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
