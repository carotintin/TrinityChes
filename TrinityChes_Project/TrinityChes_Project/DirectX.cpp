#include "DirectX.h"




ID3D11Device* g_pDevice;			// DirectX�ŃA�Z�b�g�̍쐬���s���@�\
ID3D11DeviceContext* g_pContext;	// DirectX�ŕ`��ړ���𔭍s����@�\
IDXGISwapChain* g_pSwapChain;		//�t���[���o�b�t�@�ƃf�B�X�v���C�̋��n��
ID3D11RenderTargetView* g_pRTV;		//�t���[���o�b�t�@���̂���
ID3D11BlendState* g_pBlendState;

float R = 201.0f / 255.0f;
float G = 218.0f / 255.0f;
float B = 244.0f / 255.0f;

ID3D11SamplerState* g_pSamplerState;

HRESULT InitDirectX(HWND hWnd, UINT width, UINT height, BOOL fullScreen)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = width; //�t���[���o�b�t�@�̕�
	sd.BufferDesc.Height = height; //�t���[���o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �t���[���o�b�t�@�̐F���( 8bit)
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�o�b�N�o�b�t�@�̎g�p���@
	sd.BufferCount = 1;									//�o�b�t�@�̐�
	sd.OutputWindow = hWnd;								//�֘A�t����E�B���h�E
	sd.Windowed = fullScreen ? FALSE : TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;



	


	

	//�h���C�o�̎��
	D3D_DRIVER_TYPE driverType;
	D3D_DRIVER_TYPE driverTypes[] = {
										D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE
									};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	//�@�\���x��
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = {
											D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
											D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
											D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
										};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// DirectX��Debug�ݒ�
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// �쐬�\�Ȕ͈͂�DirectX�̋@�\��L����
	HRESULT hr;
	for (UINT i = 0; i < numDriverTypes; i++)
	{
		driverType = driverTypes[i];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pDevice, &featureLevel, &g_pContext);

		if (SUCCEEDED(hr))
		{ 
			break;
		}
	}

	if (FAILED(hr)) 
	{ 
		return hr; 
	}

	// �t���[���o�b�t�@�ւ̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	// �t���[���o�b�t�@�ւ̃|�C���^�ŕR�Â��āA�����_�[�^�[�Q�b�g�r���[���쐬
	if (SUCCEEDED(hr)) 
	{
		hr = g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRTV);
		g_pContext->OMSetRenderTargets(1, &g_pRTV, nullptr); // GPU�̏o�͐�ݒ�
	}

	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pContext->RSSetViewports(1, &vp);


	//�u�����h�X�e�[�g�̍쐬
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	hr = g_pDevice->CreateBlendState(&blendDesc, &g_pBlendState);
	if (FAILED(hr)) { return hr; }

	//�u�����h�X�e�[�g�̐ݒ�
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_pContext->OMSetBlendState(g_pBlendState, blendFactor, 0xffffffff);


	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // �g��E�k�����s�����Ƃ��A�ǂ���Ԃ��邩
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // �t(��)�����̌J��Ԃ��ݒ�
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // �u(�c)�����̌J��Ԃ��ݒ�
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // �v(��)�����̌J��Ԃ��ݒ�(2D�ł��ݒ肪�K�v
	hr = g_pDevice->CreateSamplerState(&sampDesc, &g_pSamplerState);
	if (FAILED(hr)) { return hr; }
	GetContext()->PSSetSamplers(0, 1, &g_pSamplerState);


	return hr;
}


void UninitDirectX()
{
	g_pBlendState->Release();

	g_pRTV->Release();
	g_pSwapChain->SetFullscreenState(false, NULL);
	g_pSwapChain->Release();
	g_pContext->ClearState();
	g_pContext->Release();
	g_pDevice->Release();

	g_pSamplerState->Release();
}


void BeginDrawDirectX()
{
	float color[4] = { R, G, B, 1.0f };
	g_pContext->ClearRenderTargetView(g_pRTV, color);
}


void EndDrawDirectX()
{
	// �`�抮�����ɉ�ʂ֏o��
	g_pSwapChain->Present(0, 0);
}


ID3D11Device* GetDevice()// Device
{
	return g_pDevice;
}


ID3D11DeviceContext* GetContext() // Context
{
	return g_pContext;
}
