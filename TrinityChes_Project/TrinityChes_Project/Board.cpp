#include "Board.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"
#include "Player.h"
#include "GameManager.h"
#include <utility>
#include <set>


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

	m_bTurnEnd = false;

	// 初期フェーズを SETUP_PHASE に変更
	m_SelectPhase = SETUP_PHASE;
	m_pvecCandidates = _pvecCandidates;


	// === ここから：先攻後攻の決定と配置順の作成 ===
	srand((unsigned int)time(NULL));
	PLAYER_ID firstPlayer = (rand() % 2 == 0) ? PLAYER1 : PLAYER2;
	PLAYER_ID secondPlayer = (firstPlayer == PLAYER1) ? PLAYER2 : PLAYER1;

	// インデックス対応表: P1=King(0),Bishop(1),Rook(2) / P2=King(3),Bishop(4),Rook(5)
	int f_B = (firstPlayer == PLAYER1) ? 1 : 4; // 先攻のビショップ
	int s_B = (secondPlayer == PLAYER1) ? 1 : 4; // 後攻のビショップ
	int f_R = (firstPlayer == PLAYER1) ? 2 : 5; // 先攻のルーク
	int s_R = (secondPlayer == PLAYER1) ? 2 : 5; // 後攻のルーク
	int f_K = (firstPlayer == PLAYER1) ? 0 : 3; // 先攻のキング
	int s_K = (secondPlayer == PLAYER1) ? 0 : 3; // 後攻のキング

	// 配置順リストに登録 (1:ビショップ, 2:ルーク, 3:キングの交互)
	m_SetupOrder.push_back({ f_B, firstPlayer });
	m_SetupOrder.push_back({ s_B, secondPlayer });
	m_SetupOrder.push_back({ f_R, firstPlayer });
	m_SetupOrder.push_back({ s_R, secondPlayer });
	m_SetupOrder.push_back({ f_K, firstPlayer });
	m_SetupOrder.push_back({ s_K, secondPlayer });

	m_SetupIndex = 0;

	// GameManagerに最初のプレイヤーのターンをセット（ロゴ表示用）
	CGameManager::GetInstance().SetTurn(firstPlayer);

	// 全駒を使用可能状態にしておく
	for (int i = 0; i < PIECE_NUM; ++i) {
		m_pPieces[i]->SetUsable(false);
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
	UpdateSetupAreas();
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

	if (m_SelectPhase == SETUP_PHASE && m_SetupIndex < m_SetupOrder.size())
	{
		m_pPieces[m_SetupOrder[m_SetupIndex].pieceIndex]->Draw();
	}

	m_Cursor->Draw();

}

//盤面の更新処理
void CBoard::Update()
{
	m_Cursor->Update();

	switch (m_SelectPhase)
	{
	case SETUP_PHASE:
		// 毎フレーム、カーソル位置に基づくプレビューと判定を更新する
		UpdateSetupPreview();

		if (m_Cursor->GetbPress()) // 決定キーが押されたら
		{
			// m_bCanSet が true の時だけ配置処理を行う
			if (m_bCanSet)
			{
				int x, y;
				m_Cursor->GetPos(&x, &y);
				int pIdx = m_SetupOrder[m_SetupIndex].pieceIndex;

				// 駒の座標を更新し、マスにセットする
				aSquare[y][x].SetPiece(m_pPieces[pIdx]);

				// 配置されたので、これ以降「行動範囲計算」の対象にする
				m_pPieces[pIdx]->SetUsable(true);

				// 次の配置へ
				m_SetupIndex++;

				// 全ての配置が終わったかチェック
				if (m_SetupIndex >= m_SetupOrder.size())
				{
					// 配置フェーズのハイライトをすべて通常状態に戻す
					for (int sy = 0; sy < MAX_BOARD_SIZE; ++sy) {
						for (int sx = 0; sx < MAX_BOARD_SIZE; ++sx) {
							aSquare[sy][sx].SetState(CSquare::SQUARE_STATE::NORMAL);
						}
					}
					// 全駒を使用可能にする
					for (int i = 0; i < PIECE_NUM; ++i) {
						m_pPieces[i]->SetUsable(true);
					}

					// 本編ゲーム開始時に、最初のターン（先攻）のプレイヤーに戻す
					CGameManager::GetInstance().SetTurn(m_SetupOrder[0].player);

					// ゲーム開始準備
					m_SelectPhase = SELECT_PIECE;
					FindMovableArea();
				}
				else
				{
					// 次の駒を配置するプレイヤーのターンに切り替える
					CGameManager::GetInstance().SetTurn(m_SetupOrder[m_SetupIndex].player);

					// 新しい駒が置かれたので行動範囲を再計算
					FindMovableArea(false);
					UpdateSetupAreas();
				}
			}
		}
		break;


	case SELECT_PIECE:	//動かす駒を選んでる時
		if (m_Cursor->GetbPress()) //決定キーが押されたら
		{
			//カーソルの位置を持ってくる
			int x;
			int y;
			m_Cursor->GetPos(&x, &y);

			//カーソルの位置に駒があるか見る
			m_pSelectedPiece = (aSquare[y][x].GetPiece());
			if (m_pSelectedPiece != nullptr)	//あったら
			{
				// プレイヤーターンと駒の持ち主が同じか見る＆前ターンに使ってないか見る
				if (m_pSelectedPiece->GetID() == CGameManager::GetInstance().GetTurn()
					&& m_pSelectedPiece->GetUsable())
				{
					FindMovableArea(false);
					
					//その駒の移動可能範囲を出す
					MakeMovable(aSquare[y][x].GetPiece());
					m_SelectPhase = SELECT_DESTINATION;
				}

			}
		}
		break;

	case SELECT_DESTINATION:	// 駒の移動可能範囲を表示してる時
		if (m_Cursor->GetbPress())	// 決定キーが押されたら
		{
			// カーソルの位置を持ってくる
			int x;
			int y;
			m_Cursor->GetPos(&x, &y);

			// カーソルの位置が駒の移動可能範囲だったら
			if (aSquare[y][x].GetState() == CSquare::MOVABLE)
			{
				// 駒が元居た位置を空にする
				int prevX;
				int prevY;
				m_pSelectedPiece->GetPos(&prevX, &prevY);
				aSquare[prevY][prevX].SetPiece(nullptr);

				// 駒を移動させる
				m_pSelectedPiece->Move(x, y);
				// 盤面にも登録
				aSquare[y][x].SetPiece(m_pSelectedPiece);

				// 駒で塗った移動可能範囲を戻す
				ResetState(m_pSelectedPiece);

				
				// 移動させた駒を使用不可＆使用不可だった自分の駒を使用可能に
				for (int i = 0; i < PIECE_NUM; ++i)
				{
					// 自分の駒かどうかチェック
					if (m_pPieces[i]->GetID() == m_pSelectedPiece->GetID())
					{
						if (m_pPieces[i] == m_pSelectedPiece)
						{
							m_pPieces[i]->SetUsable(false); // 今動かした駒
						}
						else
						{
							m_pPieces[i]->SetUsable(true);  // それ以外の自分の駒
						}
					}
				}

				// 全駒の移動可能範囲を計算しなおす＆チェックメイト判定
				FindMovableArea();

				// ターン終了
				m_bTurnEnd = true;

				// 駒を選ぶフェイズに戻る
				m_SelectPhase = SELECT_PIECE;

			}
			else if (aSquare[y][x].GetState() == CSquare::NORMAL)
			{
				// 移動可能範囲じゃないとこを選択したら駒選択に戻る
				ResetState(m_pSelectedPiece);
				m_SelectPhase = SELECT_PIECE;
			}

		}
		break;

	default:
		break;
	}

	
}




void CBoard::FindMovableArea(bool bCheckMate)
{
	// 判定を行う場合のみフラグを初期化
	if (bCheckMate)
	{
		m_bTrinityCheckMate = false;
		m_bJibakuMate = false;
	}

	m_pvecCandidates->clear();
	PLAYER_ID myID = CGameManager::GetInstance().GetTurn();

	// 判定用データ保持
	std::set<CPiece*> targetedEnemies; // 射程に入れた敵駒のユニークリスト
	struct EnemyRangeData {
		CPiece* pPiece;
		std::vector<std::pair<int, int>> range;
	};
	std::vector<EnemyRangeData> enemyRangeList;

	bool bFatalJibaku = false;

	// 自軍のカバー範囲を「動ける駒」と「全駒」で明確に分ける
	std::set<std::pair<int, int>> myMovableCoverage; // 動ける自分の駒の射程
	std::set<std::pair<int, int>> myAllCoverage;     // 動けない駒も含めた全自分の駒の射程

	// ========================================================
	// 1. 全駒の走査と移動可能範囲の計算
	// ========================================================
	for (int i = 0; i < PIECE_NUM; ++i)
	{
		CPiece* pPiece = m_pPieces[i];
		bool isMyPiece = (pPiece->GetID() == myID);

		int curX, curY;
		pPiece->GetPos(&curX, &curY);
		std::vector<std::pair<int, int>> currentRange;

		for (int j = 0; j < 8; ++j)
		{
			int movement = pPiece->GetMovement(j);
			bool bMoveBlocked = false; // 移動がブロックされたかどうかのフラグ

			for (int k = 0; k < movement; ++k)
			{
				int nX = curX + aDirection[j][0] * (k + 1);
				int nY = curY + aDirection[j][1] * (k + 1);

				if (nX < 0 || nX >= MAX_BOARD_SIZE || nY < 0 || nY >= MAX_BOARD_SIZE) break;

				CPiece* pTarget = aSquare[nY][nX].GetPiece();

				if (isMyPiece) {
					// --- 自分の駒の処理 ---

					// まだブロックされていない場合のみカバー範囲として記録
					if (!bMoveBlocked) {
						myAllCoverage.insert({ nX, nY }); // 全駒の射程として記録
						if (pPiece->GetUsable()) {
							myMovableCoverage.insert({ nX, nY }); // 動ける駒の射程として記録
						}
					}

					if (pTarget != nullptr) {
						if (pTarget->GetID() != myID) {
							// 敵を射程に捉えた（貫通して複数カウント可能）
							targetedEnemies.insert(pTarget);

							// 動かせる駒の場合のみ、実際の移動先候補に追加（ブロックされる前の最初の一つのみ）
							if (!bMoveBlocked && pPiece->GetUsable()) {
								m_TmpCandidate.piece = pPiece; m_TmpCandidate.x = nX; m_TmpCandidate.y = nY;
								m_pvecCandidates->push_back(m_TmpCandidate);
							}

							// 敵を貫通して奥の敵も探すため、移動はブロックするがループは抜けない
							bMoveBlocked = true;
						}
						else {
							// 味方の駒だった場合
							// これ以上先へは行けないし、射線も通らないので完全に探索終了
							break;
						}
					}
					else {
						// 空きマス。動かせる駒の場合のみ、移動先候補に追加
						if (!bMoveBlocked && pPiece->GetUsable()) {
							m_TmpCandidate.piece = pPiece; m_TmpCandidate.x = nX; m_TmpCandidate.y = nY;
							m_pvecCandidates->push_back(m_TmpCandidate);
						}
					}
				}
				else {
					// --- 相手の駒の処理（自爆・新トリニティ判定用） ---
					if (pTarget != nullptr) {
						if (bCheckMate && pTarget->GetID() == myID)
						{
							// 本当の自爆（確殺）かどうかの判定
							// パターン1：敵が「次に動ける(Usable=true)」なら、次のターンで食べられるので自爆
							// パターン2：狙われている自分の駒が「今動かしたばかり(Usable=false)」なら、自分から入ったので自爆
							// ※それ以外（敵は動けず、自分は次に逃げられる状態）ならセーフ
							if (pPiece->GetUsable() || !pTarget->GetUsable())
							{
								m_bJibakuMate = true;
								bFatalJibaku = true;
							}
						}
						break; // 敵の射線は貫通させない
					}
					currentRange.push_back({ nX, nY });
				}
			}
		}
		if (!isMyPiece) enemyRangeList.push_back({ pPiece, currentRange });
	}

	// ========================================================
	// 2. 勝利条件（トリニティ）の最終判定
	// ========================================================

	if (bCheckMate)
	{
		// 最優先：自爆敗北が確定していればトリニティ判定は行わない
		if (bFatalJibaku)
		{
			return;
		}

		// 条件: 敵を3駒以上射程に入れた
		if (targetedEnemies.size() >= 3) {
			m_bTrinityCheckMate = true;
			return;
		}

		// 条件:相手の移動範囲が自分の範囲に完全重複
		for (const auto& enemy : enemyRangeList)
		{
			int ex, ey;
			enemy.pPiece->GetPos(&ex, &ey);

			if (!enemy.pPiece->GetUsable()) {
				// ① 相手の駒が「動けない」場合
				if (myMovableCoverage.count({ ex, ey })) {
					m_bTrinityCheckMate = true;
					break;
				}
			}
			else {
				// ② 相手の駒が「動ける」場合
				if (enemy.range.empty()) {
					// 逃げ場が完全にない場合、本体が（動ける自軍の駒から）狙われていればアウト
					if (myMovableCoverage.count({ ex, ey })) {
						m_bTrinityCheckMate = true;
						break;
					}
				}
				else {
					// 移動先がある場合、その「全ての移動先（逃げ道）」が塞がれているかチェック
					bool allCovered = true;
					for (const auto& pos : enemy.range) {
						if (myAllCoverage.count(pos) == 0) {
							allCovered = false;
							break;
						}
					}

					// 【修正】逃げ道が塞がれているだけでなく、現在のマス（本体）も動ける駒から狙われている時だけトリニティ！
					// （動かさずにその場に留まるという選択肢が安全なら、まだチェックメイトではないため）
					if (allCovered && myMovableCoverage.count({ ex, ey })) {
						m_bTrinityCheckMate = true;
						break;
					}
				}
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

// 駒で塗った移動可能範囲を戻す
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

// 配置フェーズ用の配置可能エリア計算・表示関数
void CBoard::UpdateSetupAreas()
{
	// 現在のターンのプレイヤーID（味方）を取得
	PLAYER_ID myID = CGameManager::GetInstance().GetTurn();

	// 1. まず全マスを初期化する
	for (int y = 0; y < MAX_BOARD_SIZE; ++y)
	{
		for (int x = 0; x < MAX_BOARD_SIZE; ++x)
		{
			// すでに別の駒があるマスは配置できないので NORMAL
			if (aSquare[y][x].ExistPiece())
			{
				aSquare[y][x].SetState(CSquare::SQUARE_STATE::NORMAL);
			}
			else
			{
				// 一旦、すべての空きマスを配置可能（MOVABLE）とする
				aSquare[y][x].SetState(CSquare::SQUARE_STATE::MOVABLE);
			}
		}
	}

	// 2. 敵の駒の行動範囲（射線）を計算し、危険地帯(NORMAL)として上書きする
	for (int i = 0; i < PIECE_NUM; ++i)
	{
		CPiece* pPiece = m_pPieces[i];

		// 使用不可（未配置）の駒、または「味方の駒」はスキップする
		// ※これにより、味方の行動範囲は MOVABLE のまま維持されます
		if (!pPiece->GetUsable() || pPiece->GetID() == myID)
		{
			continue;
		}

		// ここから敵の駒の行動範囲計算
		int currentX, currentY;
		pPiece->GetPos(&currentX, &currentY);

		// 8方向の探索
		for (int j = 0; j < 8; ++j)
		{
			int movement = pPiece->GetMovement(j);

			for (int k = 0; k < movement; ++k)
			{
				int newX = currentX + aDirection[j][0] * (k + 1);
				int newY = currentY + aDirection[j][1] * (k + 1);

				// 盤面外ならこの方向の探索を終了
				if (newX < 0 || newX >= MAX_BOARD_SIZE || newY < 0 || newY >= MAX_BOARD_SIZE) break;

				// もし空きマスなら、そこは敵の射線なので配置不可(NORMAL)にする
				if (!aSquare[newY][newX].ExistPiece())
				{
					aSquare[newY][newX].SetState(CSquare::SQUARE_STATE::NORMAL);
				}
				else
				{
					// 既存の駒（敵味方問わず）にぶつかったら、それより奥には射線が通らない
					break;
				}
			}
		}
	}
}

// カーソル位置の配置判定とNOSETプレビュー表示
void CBoard::UpdateSetupPreview()
{
	// 毎フレーム、一旦すべての配置可能エリア（NORMAL / MOVABLE）を最新状態にリセットする
	UpdateSetupAreas();

	// すでに配置がすべて終わっていれば何もしない
	if (m_SetupIndex >= m_SetupOrder.size()) return;

	int cx, cy;
	m_Cursor->GetPos(&cx, &cy);

	// 今配置しようとしている駒を取得
	int pIdx = m_SetupOrder[m_SetupIndex].pieceIndex;
	CPiece* pCurrentPiece = m_pPieces[pIdx];

	// 描画と行動範囲計算のために、駒の内部座標をカーソルに合わせる
	pCurrentPiece->Move(cx, cy);

	// 基本判定：現在のカーソル位置が「配置可能（MOVABLE）」かどうか
	m_bCanSet = (aSquare[cy][cx].GetState() == CSquare::SQUARE_STATE::MOVABLE);

	PLAYER_ID myID = pCurrentPiece->GetID();

	// カーソル位置に置いた場合の移動範囲を計算して、NOSETテクスチャを上書きする
	for (int j = 0; j < 8; ++j)
	{
		int movement = pCurrentPiece->GetMovement(j);
		for (int k = 0; k < movement; ++k)
		{
			int nx = cx + aDirection[j][0] * (k + 1);
			int ny = cy + aDirection[j][1] * (k + 1);

			if (nx < 0 || nx >= MAX_BOARD_SIZE) break;
			if (ny < 0 || ny >= MAX_BOARD_SIZE) break;

			// そのマスにすでに駒がある場合
			if (aSquare[ny][nx].ExistPiece())
			{
				CPiece* pEncountered = aSquare[ny][nx].GetPiece();
				// それが相手の駒なら、射線が通ってしまうため配置不可にする
				if (pEncountered->GetID() != myID)
				{
					m_bCanSet = false;
				}
				// それが味方の駒なら、射線を止める
				if (pEncountered->GetID() == myID)
				{
					break;
				}

			}

			// 駒がない空きマスなら、NOSETステートにして赤くハイライトする
			aSquare[ny][nx].SetState(CSquare::SQUARE_STATE::NOSET);
		}
	}
}