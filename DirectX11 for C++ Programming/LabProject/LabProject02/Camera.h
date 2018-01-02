#pragma once

struct VS_CB_VIEWPROJECTION_MATRIX
{
	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;
};

class CPlayer;

class CCamera
{
protected:
	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;

	// 뷰포트를 나타내는 멤버 변수
	D3D11_VIEWPORT m_d3dViewport;

	// 카메라 변환 행렬과 투영 변환 행렬을 위한 상수 버퍼 인터페이스 포인터
	ID3D11Buffer* m_pd3dcbViewProjection;

	// 플레이어 객체에 대한 포인터
	CPlayer* m_player;


public:
	CCamera();
	~CCamera();

	inline void setPlayer(CPlayer *pPlayer) { m_player = pPlayer; };

	// 뷰포트를 설정하는 함수
	void setViewport(ID3D11DeviceContext* pd3dDeviceConetxt, DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);

	// 카메라 변환 행렬과 투영 변환 행렬을 생성하는 함수
	void generateViewMatrix(D3DXVECTOR3 d3dxvEyePosition, D3DXVECTOR3 d3dxvLookAt, D3DXVECTOR3 d3dxvUp);
	void generateProjectionMatrix(float fNearPlaneDistance, float fFrarPlaneDistance, float fAspectRatio, float fFOVAngle);

	// 상수 버퍼를 생성하고 내용을 갱신하는 함수
	void createShaderVariables(ID3D11Device* pd3dDevice);
	void updateSHaderVariables(ID3D11DeviceContext* pd3dDeviceContext);
};

