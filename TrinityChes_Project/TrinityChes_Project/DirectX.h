#ifndef _DIRECT_X_
#define _DIRECT_X_


#include <d3d11.h>					 // DirectX�̋@�\�̗��p
#pragma comment(lib, "d3d11.lib")	 //DirectX�̋@�\�̖{��


HRESULT InitDirectX(HWND hWnd, UINT width, UINT height, BOOL fullScreen);
void UninitDirectX();
void BeginDrawDirectX();
void EndDrawDirectX();
ID3D11Device* GetDevice(); // Device
ID3D11DeviceContext* GetContext(); // Context

#endif _DIRECT_X_