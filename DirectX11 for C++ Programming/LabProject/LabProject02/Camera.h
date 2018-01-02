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

	// ����Ʈ�� ��Ÿ���� ��� ����
	D3D11_VIEWPORT m_d3dViewport;

	// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ���� ��� ���� �������̽� ������
	ID3D11Buffer* m_pd3dcbViewProjection;

	// �÷��̾� ��ü�� ���� ������
	CPlayer* m_player;


public:
	CCamera();
	~CCamera();

	inline void setPlayer(CPlayer *pPlayer) { m_player = pPlayer; };

	// ����Ʈ�� �����ϴ� �Լ�
	void setViewport(ID3D11DeviceContext* pd3dDeviceConetxt, DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);

	// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� �����ϴ� �Լ�
	void generateViewMatrix(D3DXVECTOR3 d3dxvEyePosition, D3DXVECTOR3 d3dxvLookAt, D3DXVECTOR3 d3dxvUp);
	void generateProjectionMatrix(float fNearPlaneDistance, float fFrarPlaneDistance, float fAspectRatio, float fFOVAngle);

	// ��� ���۸� �����ϰ� ������ �����ϴ� �Լ�
	void createShaderVariables(ID3D11Device* pd3dDevice);
	void updateSHaderVariables(ID3D11DeviceContext* pd3dDeviceContext);
};

