#include "VertexBuffer.h"
#include "DirectX.h"


ID3D11Buffer* CreateVertexBuffer(Vertex* vtxData, UINT vtxNum)
{
	// �o�b�t�@��� �ݒ�
	D3D11_BUFFER_DESC vtxBufDesc;
	ZeroMemory(&vtxBufDesc, sizeof(vtxBufDesc));
	vtxBufDesc.ByteWidth = sizeof(Vertex) * vtxNum; // �o�b�t�@�̑傫��
	vtxBufDesc.Usage = D3D11_USAGE_DEFAULT; // ��������ł̊Ǘ����@
	vtxBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // GPU��ł̗��p���@
	// �o�b�t�@�����f�[�^ �ݒ�
	D3D11_SUBRESOURCE_DATA vtxSubResource;
	ZeroMemory(&vtxSubResource, sizeof(vtxSubResource));
	vtxSubResource.pSysMem = vtxData; // �o�b�t�@�ɗ������ރf�[�^
	// �쐬
	HRESULT hr;
	ID3D11Buffer* pVtxBuf;
	hr = GetDevice()->CreateBuffer(&vtxBufDesc, &vtxSubResource, &pVtxBuf);
	if (FAILED(hr)) { return nullptr; }
	return pVtxBuf;
}
