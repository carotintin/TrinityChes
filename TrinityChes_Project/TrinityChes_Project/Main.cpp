#include <Windows.h>
#include <string>
#include "DirectX.h"
#include "SpriteDrawer.h"
#include "VertexBuffer.h"
#include "Game.h" 


//����邱��
//�P�FOK�@�Ֆʂ̕`��
//�Q�F��̕`��
//		����̃f�[�^�����i��̎�ށA���̈ړ��͈͂̒��ו��j
//		��OK�@���W�ϊ�
//�R�F��̈ړ��̏��������
//�S�F�l�݂̏��������
//�T�F�Q�[���̗�������
//		����U�E��U���߃t�F�[�Y
//		����̔z�u�t�F�[�Y
//			�E��Ԍ���(���N�̔ԂȂ̂�)
//		�`�u���I���܂Ŏ�Ԃ��Ƃɍs���`
//			�P���u���ꏊ�����߂�
//			�Q��u���i���̎��_�ŋl�ݔ���͒��ׂĂ����j
// 
//�`���s�����܂�܂Ł`
//		���Q�[���t�F�[�Y
//			�E��Ԍ���
//			�E�ړ��͈͂𒲂ׂ�i�ړ��ł������z��ɓ���Ă����j
//			�E���ۂɈړ�����ʒu�����߂�
//			�E��̏���
//				�P������ۂɈړ�������
//				�Q��̏�Ԃ�ς���i��x����������͘A���œ������Ȃ����[���j		
//				�R�������C�g��������	���Q�[���}�l�[�W���[
//				�S�g���j�e�B�`�F�b�N���C�g���������@���Q�[���}�l�[�W���[
// 
// 
// 
// 
//�����[���F
// 
//		�E�e�v���C���[�́u�L���O�E���[�N�E�r�V���b�v�v1�̂��g�p
//		�E ������͘A���œ������Ȃ��i��F���[�N�����[�N�͋֎~�j
//		�E �����z�u�͎��R�Ɍ��߂���i���݁j
//		�E �ǂꂩ1��ł����ꂽ�瑦����
// 
//���g���j�e�B�`�F�b�N���C�g�����F
// 
//		�E����̈ړ��͈͂ɓ�ȏ�̎����̋����ꍇ
//		�E����̍U���͈͂ɂ����ǂ��Ɉړ����Ă��ړ��͈͂��瓦����Ȃ��ꍇ
//			
//���������C�g�����F
// 
//		�E�����̓������������̈ړ��͈͂ɓ����Ă���ꍇ
//
//�i�ǂꂩ�P���ꂽ�畉���Ȃ̂ő���̋���l�鏈���͕K�v�Ȃ��j



//mm �}���`���f�B�A
//lib cpp����̃t�@�C���ɂ܂Ƃ߂��悤�Ȃ���
#pragma comment(lib, "winmm.lib")

//--- �v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	// �E�B���h�E�N���X���̍쐬
	WNDCLASSEX wcex;
	// �w�肳�ꂽ�A�h���X�̕ϐ����O�ɂ���֐�
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.hInstance = hInstance;	//�A�v���P�[�V�����̎��ʔԍ�
	wcex.lpszClassName = "Class Name";
	wcex.lpfnWndProc = WndProc;		//�E�B���h�E�v���V�[�W���̐ݒ�
	wcex.style = CS_HREDRAW | CS_VREDRAW; // �E�B���h�E�̋���
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // �A�v���̃A�C�R��
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // �}�E�X�̃A�C�R��
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // �w�i�̐F

	// �E�B���h�E�N���X���̓o�^
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "�E�B���h�E�N���X�̓o�^�Ɏ��s", "�G���[", MB_OK);
		return 0;
	}

	// �E�B���h�E�̍쐬
	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, // �E�B���h�E�̌����ڇ@
		wcex.lpszClassName, "�Ղ�Ղ�", // �^�C�g���o�[�ɕ\�����镶��
		WS_CAPTION | WS_SYSMENU, // �E�B���h�E�̌����ڇA
		CW_USEDEFAULT, 
		CW_USEDEFAULT, // �E�B���h�E�̕\���ʒu
		960, 
		540, // �E�B���h�E�̑傫��
		HWND_DESKTOP, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, "�E�B���h�E�̍쐬�Ɏ��s", "�G���[", MB_OK);
		return 0;
	}
	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// DirectX�̏���������
	if (FAILED(InitDirectX(hWnd, 960, 540, false)))
	{
		MessageBox(hWnd, "DirectX�̏������Ɏ��s","�G���[", MB_OK);
		return 0;
	}

	// �X�v���C�g�̏�����
	InitSpriteDrawer(GetDevice(), GetContext(), 960, 540);

	// �Q�[���̏���������
	if (!InitGame()) 
	{ 
		MessageBox(hWnd, "Game�̏������Ɏ��s", "�G���[", MB_OK);
		return 0;
	}

	//  FPS�̏���
	DWORD oldTime;    // �O�̎��s����
	DWORD nowTime;    // ���݂̎���
	DWORD fpsCount = 0; // 1�b�Ԃ̏�����
	DWORD fpsOldTime;	//FPS�v���̑O�̎��s����
	timeBeginPeriod(1);
	oldTime = timeGetTime();
	fpsOldTime = timeGetTime();


	// ���b�Z�[�W���[�v
	MSG message;
	while (1) 
	{
		//���b�Z�[�W�L���[�Ƀ��b�Z�[�W�����邩
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE)) 
		{
			//�E�B���h�E����Ă���ƃG���[����������
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
			//FPS�̔���
			nowTime = timeGetTime();
			if (nowTime - oldTime >= 1000 / 60)
			{
				UpdateGame();		//�X�V����
				BeginDrawDirectX();
				DrawGame();			//�`�揈��
				EndDrawDirectX();

				//FPS�̏����X�V
				oldTime = nowTime;
				fpsCount++;
			}

			//FPS�J�E���g�X�V�^�C�~���O����
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
	UninitSpriteDrawer(); // �I�������iUninitDirectX�����O�ɒǋL
	UninitDirectX();
	
	return 0;

	
}

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//���{�^�����������Ƃ��̏���
	case WM_CLOSE:
		if (IDNO == MessageBox(hWnd, "��߂�H", "��߂��", MB_YESNO | MB_ICONWARNING ))
		{
			return 0;
			//DefWindowProc����������Ȃ��Ȃ�
			//���������Ă��E�B���h�E�����Ȃ��Ȃ�
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}