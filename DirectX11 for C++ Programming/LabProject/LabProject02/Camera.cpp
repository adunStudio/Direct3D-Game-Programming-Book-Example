#include "stdafx.h"
#include "Camera.h"


CCamera::CCamera()
{
	m_player = nullptr;
	D3DXMatrixIdentity(&m_d3dxmtxView);
	D3DXMatrixIdentity(&m_d3dxmtxProjection);
}


CCamera::~CCamera()
{
	if (m_pd3dcbViewProjection)
		m_pd3dcbViewProjection->Release();
}

// ����Ʈ�� �����ϴ� �Լ�
void CCamera::setViewport(ID3D11DeviceContext* pd3dDeviceContext, DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = float(xStart);
	m_d3dViewport.TopLeftY = float(yStart);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;

	pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� �����ϴ� �Լ�

// ī�޶��� ��ġ ����, ī�޶� �ٶ󺸴� ����, ī�޶��� Up����(���� y�� ����)
void CCamera::generateViewMatrix(D3DXVECTOR3 d3dxvEyePosition, D3DXVECTOR3 d3dxvLookAt, D3DXVECTOR3 d3dxvUp)
{
	D3DXMatrixLookAtLH(&m_d3dxmtxView, &d3dxvEyePosition, &d3dxvLookAt, &d3dxvUp);
}

// ī�޶� ��ǥ���� �� ��� �Ÿ�, �� ���Ÿ�, ȭ���� ��Ⱦ��, ī�޶� FOV ���� ��ġ����
void CCamera::generateProjectionMatrix(float fNearPlaneDistance, float fFrarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	D3DXMatrixPerspectiveFovLH(&m_d3dxmtxProjection, (float)D3DXToRadian(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFrarPlaneDistance);
}

// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ���̴��� �����ϱ� ���� ����ü�� ���Ͽ� ��� ���۸� �����ϴ� �Լ�
void CCamera::createShaderVariables(ID3D11Device* pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_VIEWPROJECTION_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// �Է�, �Է�, ���
	pd3dDevice->CreateBuffer(&bd, nullptr, &m_pd3dcbViewProjection);
}

// ī�޶� ��ȯ ��İ� ������ȯ ����� ���̴��� �����ϱ� ���� ����ü�� ��� ���۸� �����ϰ� ����̽� ���ؽ�Ʈ ���Կ� �����ϴ� �Լ�
// ��� ������ �޸� �ּҸ� �����ͼ� ī�޶� ��ȯ ��İ� ���� ��ȯ ����� �����Ѵ�.
void CCamera::updateSHaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{

	// ���� ���ҽ� �����Ϳ� �����ϱ� ���� ����ü
	// pData �����Ϳ� ���� ������
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	// ���� ���ҽ��� ���Ե� �����Ϳ� ���� �����͸� ������� �Լ� (�� �Լ��� ȣ���ϸ� GPU�� ���� ���ҽ��� ������ �� ���� �ȴ�.)
	// ID3D11Resource �������̽� ������, ���� ���ҽ� �ε���, ���ҽ��� ���� CPU�� ����, GPU�� ���ҽ��� �����ϰ� ���� �� CPU�� ��F�� �� ���ΰ��� ��Ÿ���� ���, ���ε� ���� ���ҽ��� ���� ������
	pd3dDeviceContext->Map(m_pd3dcbViewProjection, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

	VS_CB_VIEWPROJECTION_MATRIX* pcbViewProjection = (VS_CB_VIEWPROJECTION_MATRIX*)d3dMappedResource.pData;

	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, &m_d3dxmtxView);
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, &m_d3dxmtxProjection);

	// �޸� �ּ� ����� ��ȯ�Ѵ�.
	pd3dDeviceContext->Unmap(m_pd3dcbViewProjection, 0);

	// ��� ���� ����(VS_SLOT_VIEWPROJECTION_MATRIX)�� �����Ѵ�.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_VIEWPROJECTION_MATRIX, 1, &m_pd3dcbViewProjection);
}