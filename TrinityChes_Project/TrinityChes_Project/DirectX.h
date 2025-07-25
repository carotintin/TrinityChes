#ifndef _DIRECT_X_
#define _DIRECT_X_


#include <d3d11.h>					 // DirectXの機能の利用
#pragma comment(lib, "d3d11.lib")	 //DirectXの機能の本体


HRESULT InitDirectX(HWND hWnd, UINT width, UINT height, BOOL fullScreen);
void UninitDirectX();
void BeginDrawDirectX();
void EndDrawDirectX();
ID3D11Device* GetDevice(); // Device
ID3D11DeviceContext* GetContext(); // Context

#endif _DIRECT_X_