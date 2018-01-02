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

// 뷰포트를 설정하는 함수
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

// 카메라 변환 행렬과 투영 변환 행렬을 생성하는 함수

// 카메라의 위치 벡터, 카메라가 바라보는 지점, 카메라의 Up벡터(로컬 y축 벡터)
void CCamera::generateViewMatrix(D3DXVECTOR3 d3dxvEyePosition, D3DXVECTOR3 d3dxvLookAt, D3DXVECTOR3 d3dxvUp)
{
	D3DXMatrixLookAtLH(&m_d3dxmtxView, &d3dxvEyePosition, &d3dxvLookAt, &d3dxvUp);
}

// 카메라 좌표계의 근 평면 거리, 원 평면거리, 화면의 종횡비, 카메라 FOV 각도 위치벡터
void CCamera::generateProjectionMatrix(float fNearPlaneDistance, float fFrarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	D3DXMatrixPerspectiveFovLH(&m_d3dxmtxProjection, (float)D3DXToRadian(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFrarPlaneDistance);
}

// 카메라 변환 행렬과 투영 변환 행렬을 셰이더로 전달하기 위한 구조체에 대하여 상수 버퍼를 생성하는 함수
void CCamera::createShaderVariables(ID3D11Device* pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_VIEWPROJECTION_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 입력, 입력, 출력
	pd3dDevice->CreateBuffer(&bd, nullptr, &m_pd3dcbViewProjection);
}

// 카메라 변환 행렬과 투영변환 행렬을 셰이더로 전달하기 위한 구조체의 상수 버퍼를 갱신하고 디바이스 컨텍스트 슬롯에 연결하는 함수
// 상수 버퍼의 메모리 주소를 가져와서 카메라 변환 행렬과 투영 변환 행렬을 복사한다.
void CCamera::updateSHaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{

	// 서브 리소스 데이터에 접근하기 위한 구조체
	// pData 데이터에 대한 포인터
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	// 서브 리소스에 포함된 데이터에 대한 포인터를 얻기위한 함수 (이 함수를 호출하면 GPU는 서브 리소스에 접근할 수 없게 된다.)
	// ID3D11Resource 인터페이스 포인터, 서브 리소스 인덱스, 리소스에 대한 CPU의 권한, GPU가 리소스에 접근하고 잇을 때 CPU가 어덯게 할 것인가를 나타내는 상수, 맵핑된 서브 리소스에 대한 포인터
	pd3dDeviceContext->Map(m_pd3dcbViewProjection, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

	VS_CB_VIEWPROJECTION_MATRIX* pcbViewProjection = (VS_CB_VIEWPROJECTION_MATRIX*)d3dMappedResource.pData;

	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, &m_d3dxmtxView);
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, &m_d3dxmtxProjection);

	// 메모리 주소 사용을 반환한다.
	pd3dDeviceContext->Unmap(m_pd3dcbViewProjection, 0);

	// 상수 버퍼 슬롯(VS_SLOT_VIEWPROJECTION_MATRIX)에 설정한다.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_VIEWPROJECTION_MATRIX, 1, &m_pd3dcbViewProjection);
}