#include "VertexBuffer.h"
#include "DirectX.h"


ID3D11Buffer* CreateVertexBuffer(Vertex* vtxData, UINT vtxNum)
{
	// バッファ情報 設定
	D3D11_BUFFER_DESC vtxBufDesc;
	ZeroMemory(&vtxBufDesc, sizeof(vtxBufDesc));
	vtxBufDesc.ByteWidth = sizeof(Vertex) * vtxNum; // バッファの大きさ
	vtxBufDesc.Usage = D3D11_USAGE_DEFAULT; // メモリ上での管理方法
	vtxBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // GPU上での利用方法
	// バッファ初期データ 設定
	D3D11_SUBRESOURCE_DATA vtxSubResource;
	ZeroMemory(&vtxSubResource, sizeof(vtxSubResource));
	vtxSubResource.pSysMem = vtxData; // バッファに流し込むデータ
	// 作成
	HRESULT hr;
	ID3D11Buffer* pVtxBuf;
	hr = GetDevice()->CreateBuffer(&vtxBufDesc, &vtxSubResource, &pVtxBuf);
	if (FAILED(hr)) { return nullptr; }
	return pVtxBuf;
}
