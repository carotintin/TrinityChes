#include <Windows.h>
#include <string>
#include "DirectX.h"
#include "SpriteDrawer.h"
#include "VertexBuffer.h"
#include "Game.h" 


//●やること
//１：OK　盤面の描画
//２：駒の描画
//		→駒のデータを作る（駒の種類、その移動範囲の調べ方）
//		→OK　座標変換
//３：駒の移動の処理を作る
//４：詰みの条件を作る
//５：ゲームの流れを作る
//		→先攻・後攻決めフェーズ
//		→駒の配置フェーズ
//			・手番決定(今誰の番なのか)
//		～置き終わるまで手番ごとに行う～
//			１駒を置く場所を決める
//			２駒置く（この時点で詰み判定は調べておく）
// 
//～勝敗が決まるまで～
//		→ゲームフェーズ
//			・手番決定
//			・移動範囲を調べる（移動できる候補を配列に入れていく）
//			・実際に移動する位置を決める
//			・駒の処理
//				１駒を実際に移動させる
//				２駒の状態を変える（一度動かした駒は連続で動かせないルール）		
//				３自爆メイトちぇっく	←ゲームマネージャー
//				４トリニティチェックメイトちぇっく　←ゲームマネージャー
// 
// 
// 
// 
//●ルール：
// 
//		・各プレイヤーは「キング・ルーク・ビショップ」1体ずつ使用
//		・ 同じ駒は連続で動かせない（例：ルーク→ルークは禁止）
//		・ 初期配置は自由に決められる（交互）
//		・ どれか1駒でも取られたら即負け
// 
//●トリニティチェックメイト条件：
// 
//		・相手の移動範囲に二つ以上の自分の駒がある場合
//		・相手の攻撃範囲にいる駒がどこに移動しても移動範囲から逃れられない場合
//			
//●自爆メイト条件：
// 
//		・自分の動かした駒が相手の移動範囲に入っている場合
//
//（どれか１つ取られたら負けなので相手の駒を獲る処理は必要ない）



//mm マルチメディア
//lib cppを一つのファイルにまとめたようなもの
#pragma comment(lib, "winmm.lib")

//--- プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	// ウィンドウクラス情報の作成
	WNDCLASSEX wcex;
	// 指定されたアドレスの変数を０にする関数
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.hInstance = hInstance;	//アプリケーションの識別番号
	wcex.lpszClassName = "Class Name";
	wcex.lpfnWndProc = WndProc;		//ウィンドウプロシージャの設定
	wcex.style = CS_HREDRAW | CS_VREDRAW; // ウィンドウの挙動
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // アプリのアイコン
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // マウスのアイコン
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 背景の色

	// ウィンドウクラス情報の登録
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "ウィンドウクラスの登録に失敗", "エラー", MB_OK);
		return 0;
	}

	// ウィンドウの作成
	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, // ウィンドウの見た目①
		wcex.lpszClassName, "ぷよぷよ", // タイトルバーに表示する文字
		WS_CAPTION | WS_SYSMENU, // ウィンドウの見た目②
		CW_USEDEFAULT, 
		CW_USEDEFAULT, // ウィンドウの表示位置
		960, 
		540, // ウィンドウの大きさ
		HWND_DESKTOP, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, "ウィンドウの作成に失敗", "エラー", MB_OK);
		return 0;
	}
	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// DirectXの初期化処理
	if (FAILED(InitDirectX(hWnd, 960, 540, false)))
	{
		MessageBox(hWnd, "DirectXの初期化に失敗","エラー", MB_OK);
		return 0;
	}

	// スプライトの初期化
	InitSpriteDrawer(GetDevice(), GetContext(), 960, 540);

	// ゲームの初期化処理
	if (!InitGame()) 
	{ 
		MessageBox(hWnd, "Gameの初期化に失敗", "エラー", MB_OK);
		return 0;
	}

	//  FPSの処理
	DWORD oldTime;    // 前の実行時間
	DWORD nowTime;    // 現在の時間
	DWORD fpsCount = 0; // 1秒間の処理数
	DWORD fpsOldTime;	//FPS計測の前の実行時間
	timeBeginPeriod(1);
	oldTime = timeGetTime();
	fpsOldTime = timeGetTime();


	// メッセージループ
	MSG message;
	while (1) 
	{
		//メッセージキューにメッセージがあるか
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE)) 
		{
			//ウィンドウを閉じているとエラーが発生する
			if (!GetMessage(&message, NULL, 0, 0)) 
			{
				break;
			}
			else
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else
		{
			//FPSの判定
			nowTime = timeGetTime();
			if (nowTime - oldTime >= 1000 / 60)
			{
				UpdateGame();		//更新処理
				BeginDrawDirectX();
				DrawGame();			//描画処理
				EndDrawDirectX();

				//FPSの情報を更新
				oldTime = nowTime;
				fpsCount++;
			}

			//FPSカウント更新タイミング判定
			if (nowTime - fpsOldTime >= 1000)
			{
				#ifdef _DEBUG
				std::string txtFps = std::to_string(fpsCount);
				txtFps = "FPS:" + txtFps;
				SetWindowText(hWnd, txtFps.c_str());
				#endif

				fpsCount = 0;
				fpsOldTime = nowTime;
			}
		}
	}


	timeEndPeriod(1);
	UninitSpriteDrawer(); // 終了処理（UninitDirectXよりも前に追記
	UninitDirectX();
	
	return 0;

	
}

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//罰ボタンを押したときの処理
	case WM_CLOSE:
		if (IDNO == MessageBox(hWnd, "やめる？", "やめるは", MB_YESNO | MB_ICONWARNING ))
		{
			return 0;
			//DefWindowProcが処理されなくなる
			//罰を押してもウィンドウが閉じなくなる
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}